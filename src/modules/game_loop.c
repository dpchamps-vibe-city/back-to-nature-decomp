/*
 * game_loop — the game's entry point and the loop that runs every scene.
 *
 * main() initializes the hardware and libraries, then dispatches forever on
 * the current scene ID: each case loads the scene's overlay, installs its
 * update callback and runs RunSceneUpdateLoop until RequestSceneChange ends
 * it. The loop paces frames on VSync, swaps the two render contexts and frame
 * ordering tables, and calls the scene callback once per frame. Around them
 * sit the scene-selection, update-timing and packet-work interfaces the
 * overlays call, the asset-scratch bounds, and the render-context and
 * graphics-heap setup the loop drives.
 *
 * One unit, 0x80011370-0x800129E8. Its members share the small-data cluster
 * 0x8005E274-0x8005E3C0 through $gp, which only a unit that declares the data
 * can do; functions outside reach those symbols absolutely, and no member
 * reaches absolutely a symbol another member reaches through $gp. The
 * evidence is in notes/file-groupings.md.
 */

#include "common.h"
#include "packet_work.h"
#include "psyq/types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "psyq/libetc.h"
#include "psyq/kernel.h"
#include "psyq/libapi.h"
#include "psyq/libcd.h"
#include "psyq/libmcrd.h"
#include "psyq/libmcx.h"
#include "screen_fade.h"
#include "pad_input_setup.h"
#include "wide_screen_fade.h"
#include "update_timing.h"
#include "pad_input_update.h"
#include "include_asm.h"
#include "scene_selection.h"
#include "asset_scratch.h"
#include "graphics_heap.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "pad_actuator_sequences.h"
#include "aux_sprite_display.h"
#include "camera_offset.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "ovl08_scene_select_menu.h"
#include "ovl28_sprite_sequence_scene.h"
#include "random.h"
#include "vram_clear.h"
#include "display_mask.h"
#include "debug_break.h"
#include "render_packet_pool.h"
#include "state_memory_init.h"
#include "scene_phase.h"

/* SceneEntryHook is declared void in functions.h but all call sites pass
   an s32 argument (0 or 1). The callee body is empty and ignores it,
   but the caller must match the target argument setup. The return type is
   s32 because the target never allocates $v0 to a temporary born after this
   call — the call site carries a $v0 definition. */
void SceneEntryHook(s32 arg0);

/* SDK callees declared in functions.h (not included to avoid type conflicts) */
void SetVal8005E278(s32 arg0);

/* Game-specific callees (signatures from functions.h / target analysis) */
void RunSceneUpdateLoop(void);
void ResetActiveRenderContext(void);
void SelectRenderConfiguration(s32 arg0);
void CopyRenderContextBackup(s32 arg0, s32 arg1);
void InitPads(void);
/* func_800145F0 ends with `addu $v0,$zero,$zero` before `jr $ra` — it
   returns 0, and the target's callers keep $v0 out of post-call temporaries. */
void func_80017A48(u32 arg0);
void func_80017A70(u32 arg0);

/* Globals accessed by this function */
/* D_80010098 / D_8001009C: the target loads these with the unsplit assembler
   macro (lui $a2,%hi / lw $a2,%lo($a2) — one register). That form needs a
   scalar declaration: at or below -G8 cc1 sets SYMBOL_REF_FLAG, so
   mips_check_split() leaves the address unsplit. A larger declaration clears
   the flag and cc1 splits the address across two registers instead, which no
   source shape or allocation can undo (local-alloc's combine_regs() only
   records register suggestions for reg-to-reg copies, so the HIGH temp can
   never be tied to $a2).
   These symbols sit 0x4E1DC from $gp, far outside the GP window. Nothing has
   to widen anything for that: maspsx forces -G0 on GNU as, so as makes no
   small-data decision, and a symbol this translation unit does not define is
   addressed absolutely whatever its declared size. */
extern s32 D_80010098;
extern s32 D_8001009C;
s32 D_8005E27C;
s32 D_8005E284;
s32 D_8005E288;
void (*D_8005E384)(void);
s32 D_8005E388;
s32 D_8005E398;
s32 gCurrentSceneId;
s32 gActiveRenderContextIndex;
struct PadInputState *gPad0Input;
struct PadInputState *gPad1Input;
s32 *gFrameOrderingTable;
struct_8005E3C0 *gActiveRenderContext;

/* D_8005E5D8 and D_8005E5E8 declared in globals_override.h as large arrays
   for absolute lui/lw addressing under -G8 */

/* Jump table for the main switch */
extern s32 jtbl_80010008[0x15];

/*
 * main — game entry / main loop (557 target instructions)
 *
 * Initialization followed by an infinite loop with a 0x15-entry switch on
 * gCurrentSceneId (scene selector). Each case calls func_800145F0 (scene
 * transition) and installs the scene's update callback in D_8005E384, which
 * RunSceneUpdateLoop calls every frame; some also set D_8005E288 (one-shot
 * flag) or D_80070CC0. The D_8005E3B4 pointer toggles between D_8005E5D8 and
 * D_8005E5D8 + 8.
 *
 * It is spelled `main` because cc1 gives `main` its prologue call to __main
 * (the empty function at 0x80011270): the original source never wrote it.
 *
 * The switch tails are cross-jumped by the compiler into four shared blocks,
 * and which tail a case reaches is the primary evidence for its statement
 * list (0x8001198C / 0x80011B3C store D_8005E384 then clear D_8005E288;
 * 0x80011C04 stores D_80070CC0 and falls into 0x80011C10, which stores only
 * D_8005E384). Cases 2, 6, 14, 15, 19 and 20 reach a tail that does not clear
 * D_8005E288, so they must not end with that store.
 *
 */

int main(void) {
    char *env_base;
    char *buf;
    char *far_base;
    s32 *temp_s2;

    gPad0Input = (struct PadInputState *)&gPadInputStateStorage;
    gPad1Input = (struct PadInputState *)((char *)&gPadInputStateStorage + 0x38);
    D_8005E388 = 0;
    ResetCallback();
    CdInit();
    SetMem(2);
    VSync(3);
    VSync(3);
    VSync(3);
    VSync(3);
    VSync(3);
    VSync(3);
    VSync(3);
    VSync(3);
    VSync(3);
    VSync(3);
    ClearVramRectAndWait(0, 0, 0x280, 0x1E0, 0, 0, 0);
    do {
    } while (DrawSync(1) != 0);
    InitMasterVolumeRamp();
    InitSoundDriver();
    SetVideoMode(0);
    ResetGraph(0);
    gCurrentSceneId = 0xD;
    CdInitArchiveIndex();
    env_base = (char *)&gRenderContexts;
    SetDefDrawEnv((DRAWENV *)env_base, 0, 0, 0x140, 0xF0);
    temp_s2 = (s32 *)(env_base + 0x134);
    SetDefDrawEnv((DRAWENV *)temp_s2, 0, 0xF0, 0x140, 0xF0);
    SetDefDispEnv((DISPENV *)(env_base + 0x5C), 0, 0xF0, 0x140, 0xF0);
    SetDefDispEnv((DISPENV *)(env_base + 0x190), 0, 0, 0x140, 0xF0);
    *(s16 *)(env_base + 0x8) = 0;
    *(s16 *)(env_base + 0xA) = 0;
    *(s16 *)(env_base + 0x13C) = 0;
    *(s16 *)(env_base + 0x13E) = 0xF0;
    *(s16 *)(env_base + 0x198) = 0;
    *(s16 *)(env_base + 0x64) = 0;
    *(s16 *)(env_base + 0x19A) = 0;
    *(s16 *)(env_base + 0x66) = 0;
    SetGraphDebug(0);
    VSync(3);
    VSync(3);
    InitGeom();
    SetGeomOffset(0, 0);
    SetGeomScreen(0x3E8);
    GsInitGraph(0x140, 0xF0, 0, 0, 0);
    GsDefDispBuff(0, 0, 0, 0);
    GsSetOffset(0, 0);
    GsSetDrawBuffOffset();
    GsInit3D();
    MemCardInit(0);
    MemCardStart();
    McxStartCom();
    InitPads();
    ResetPadInputState();
    ClearVramRectAndWait(0, 0, 0x3FF, 0x1FF, 0xFF, 0xFF, 0xFF);
    VSyncCallback((void (*)(void))&D_80011334);
    buf = (char *)&D_8005E5D8;
    gActiveRenderContext = (struct_8005E3C0 *)temp_s2;
    gActiveRenderContextIndex = (char *)temp_s2 != env_base;
    if (gFrameOrderingTable == (s32 *)buf) {
        buf += 8;
    }
    gFrameOrderingTable = (s32 *)buf;
    SelectRenderConfiguration(0);
    ResetActiveRenderContext();
    InitCameraOffsetState();
    InitRenderPacketPools();
    InitStateMemory();
    InitSoundOutputAndCdFiles();
    OpenSoundSequencesAndStart();
    func_80017A48(3);
    func_80017A70(3);
    D_8005E398 = 0;

    while (1) {
        if (D_8005E398 == 1) {
            CopyRenderContextBackup(1, 0);
        }
        SelectRenderConfiguration(0);
        if (D_8005E27C == 1) {
            ClearVramRectAndWait(0, 0, 0x140, 0x1E0, 0, 0, 0);
        }
        SetPadRepeatTiming(0x14, 7);
        SetPadSecondaryRepeatTiming(0x14, 7);
        D_8005E284 = 1;

        /* Dispatch bounds are unsigned even though the selection API uses s32. */
        if ((u32)gCurrentSceneId < 0x15) {
            switch ((u32)gCurrentSceneId) {
            case 0:
                SceneEntryHook(1);
                CdLoadArchiveRangeBlocking(8, 9, D_80010098, D_80010098);
                D_8005E384 = Ovl08UpdateSceneSelectMenu;
                D_8005E288 = 0;
                break;

            case 1:
                SceneEntryHook(0);
                CdLoadArchiveRangeBlocking(0xA, 0xB, D_80010098, D_80010098);
                D_8005E288 = 0;
                D_8005E384 = (void (*)(void))&D_800B889C;
                break;

            case 2:
                if (D_8005E288 == 0) {
                    ResetAuxSpriteDisplay();
                    SceneEntryHook(0);
                    CdLoadArchiveRangeBlocking(0xB, 0xC, D_80010098, D_80010098);
                    D_8005E288 = 1;
                    ResetSceneDispatchState();
                }
                D_8005E384 = (void (*)(void))&D_800BBC34;
                break;

            case 3:
                SelectRenderConfiguration(1);
                SceneEntryHook(0);
                CdLoadArchiveRangeBlocking(0x11, 0x12, D_80010098, D_80010098);
                D_8005E384 = (void (*)(void))&D_800B7E38;
                D_8005E288 = 0;
                break;

            case 4:
                SceneEntryHook(0);
                CdLoadArchiveRangeBlocking(0x13, 0x14, D_80010098, D_80010098);
                D_8005E384 = (void (*)(void))&D_800B7ED8;
                D_8005E288 = 0;
                break;

            case 5:
                SceneEntryHook(0);
                CdLoadArchiveRangeBlocking(0x15, 0x16, D_80010098, D_80010098);
                D_8005E384 = (void (*)(void))&D_800B7E3C;
                D_8005E288 = 0;
                break;

            case 6:
                if (D_8005E288 == 0) {
                    SceneEntryHook(0);
                    CdLoadArchiveRangeBlocking(0xB, 0xC, D_80010098, D_80010098);
                    D_8005E288 = 1;
                }
                SetVal8005E278(1);
                gScenePhase = 4;
                D_8005E384 = (void (*)(void))&D_800BBC34;
                break;

            case 7:
                SelectRenderConfiguration(1);
                CopyRenderContextBackup(0, 1);
                SceneEntryHook(0);
                CdLoadArchiveRangeBlocking(0xF, 0x10, D_8001009C, D_8001009C);
                gSceneSubphase = 0;
                D_8005E384 = (void (*)(void))&D_8012E2CC;
                break;

            case 8:
                SelectRenderConfiguration(1);
                CopyRenderContextBackup(0, 1);
                SceneEntryHook(0);
                CdLoadArchiveRangeBlocking(0xF, 0x10, D_8001009C, D_8001009C);
                gSceneSubphase = 0;
                D_8005E384 = (void (*)(void))&D_8012E520;
                break;

            case 10:
                SelectRenderConfiguration(1);
                CopyRenderContextBackup(0, 1);
                SceneEntryHook(0);
                CdLoadArchiveRangeBlocking(0xF, 0x10, D_8001009C, D_8001009C);
                gSceneSubphase = 0;
                D_8005E384 = (void (*)(void))&D_8012E7C8;
                break;

            case 11:
                SelectRenderConfiguration(1);
                SceneEntryHook(0);
                CdLoadArchiveRangeBlocking(0x17, 0x18, D_80010098, D_80010098);
                D_8005E384 = (void (*)(void))&D_800B7EA4;
                D_8005E288 = 0;
                break;

            case 12:
                SceneEntryHook(0);
                CdLoadArchiveRangeBlocking(0x19, 0x1A, D_80010098, D_80010098);
                D_8005E384 = (void (*)(void))&D_800B7EB4;
                D_8005E288 = 0;
                break;

            case 13:
                SceneEntryHook(0);
                CdLoadArchiveRangeBlocking(0x1B, 0x1C, D_80010098, D_80010098);
                ResetSceneDispatchState();
                D_8005E384 = (void (*)(void))&D_800B7EEC;
                D_8005E288 = 0;
                break;

            case 14:
                if (D_8005E288 == 0) {
                    ResetAuxSpriteDisplay();
                    SceneEntryHook(0);
                    CdLoadArchiveRangeBlocking(0xB, 0xC, D_80010098, D_80010098);
                    D_8005E288 = 1;
                }
                gScenePhase = 5;
                D_8005E384 = (void (*)(void))&D_800BBC34;
                break;

            case 15:
                if (D_8005E288 == 0) {
                    SceneEntryHook(0);
                    CdLoadArchiveRangeBlocking(0xB, 0xC, D_8001009C, D_8001009C);
                    D_8005E288 = 1;
                }
                gScenePhase = 6;
                D_8005E384 = (void (*)(void))&D_800BBC34;
                break;

            case 16:
                SceneEntryHook(0);
                CdLoadArchiveRangeBlocking(0x1C, 0x1D, D_80010098, D_80010098);
                ResetSceneDispatchState();
                D_8005E288 = 0;
                D_8005E384 = Ovl28UpdateSpriteSequenceScene;
                break;

            case 17:
                if (D_8005E288 == 0) {
                    CdLoadArchiveRangeBlocking(0xB, 0xC, D_80010098, D_80010098);
                    D_8005E288 = 1;
                    DisableAuxSpriteDisplay();
                }
                SelectRenderConfiguration(1);
                CopyRenderContextBackup(0, 0);
                CdLoadArchiveRangeBlocking(0x1E, 0x1F, D_8001009C, D_8001009C);
                D_8005E384 = (void (*)(void))&D_8012F084;
                break;

            case 18:
                SceneEntryHook(0);
                CdLoadArchiveRangeBlocking(0x1F, 0x20, D_80010098, D_80010098);
                ResetSceneDispatchState();
                D_8005E384 = (void (*)(void))&D_800B7FCC;
                D_8005E288 = 0;
                break;

            case 19:
                if (D_8005E288 == 0) {
                    SceneEntryHook(0);
                    CdLoadArchiveRangeBlocking(0xB, 0xC, D_80010098, D_80010098);
                    D_8005E288 = 1;
                    gScenePhase = 4;
                } else {
                    BeginMasterVolumeFadeIn(0xA);
                    BeginScreenFadeDecrease(0xA, 0, 2);
                    far_base = (char *)&D_8007AFF0;
                    *(s32 *)(far_base + 0x254A0) = 0;
                }
                D_8005E384 = (void (*)(void))&D_800BBC34;
                break;

            case 20:
                SceneEntryHook(0);
                CdLoadArchiveRangeBlocking(0xB, 0xC, D_80010098, D_80010098);
                D_8005E288 = 1;
                ResetSceneDispatchState();
                gScenePhase = 7;
                D_8005E384 = (void (*)(void))&D_800BBC34;
                break;
            }
        }

        RunSceneUpdateLoop();
    }
}

/* GP-relative globals this TU defines (target accesses all via %gp_rel).
 * Tentative definitions so cc1 keeps them inside the -G8 small-data range. */
s32 D_8005E38C;
s32 D_8005E390;
s32 gSceneUpdateLoopActive;

typedef struct {
    char pad_000[0x8];
    s32 field_008;
} struct_8006C838_view_8;

extern void PresentFrame(void);

void RunSceneUpdateLoop(void) {
    char *scene_ptr;
    s32 tmp;
    s32 saved;
    s32 *toggle_ptr;

    D_8005E390 = 0;
    D_8005E38C = 0;
    gSceneUpdateLoopActive = 1;
    SetUpdateTimingMode(2);
    do {
    } while (DrawSync(1) != 0);
    if (gSceneUpdateLoopActive == 1) {
        do {
            Rand(0xFFFF);
            UpdatePadActuatorSequences();
            PollPadInputs();
            UpdateMasterVolumeAndSoundTransfer();
            if (IsAuxSpriteDisplayEnabled() == 1) {
                D_8005E384();
                DrawSync(0);
                VSync(2);
            } else {
                EnableDisplayOutput();
                tmp = (s32)gActiveRenderContext;
                scene_ptr = (char *)&gRenderContexts;
                saved = (s32)scene_ptr;
                if ((s32)scene_ptr == tmp) {
                    scene_ptr += 0x134;
                }
                gActiveRenderContext = (struct_8005E3C0 *)scene_ptr;
                gActiveRenderContextIndex = (s32)((s32)scene_ptr != saved);
                toggle_ptr = (s32 *)&D_8005E5D8;
                if (gFrameOrderingTable == &D_8005E5D8) {
                    toggle_ptr = (s32 *)((char *)&D_8005E5D8 + 8);
                }
                gFrameOrderingTable = toggle_ptr;
                ResetActiveRenderContext();
                UpdateAndDrawScreenFade();
                UpdateAndDrawWideScreenFade();
                D_8005E384();
                ((struct_8006C838_view_8 *)&D_8006C838)->field_008 += 1;
                D_8005E38C = VSync(1);
                DrawSync(0);
                D_8005E390 = VSync(1);
                D_8005E388 = (s32)PresentFrame;
                VSync(2);
            }
        } while (gSceneUpdateLoopActive == 1);
    }
}

/* Not decompiled yet. No code calls PresentFrame directly, so the call graph
 * classes it dead, but RunSceneUpdateLoop stores its address in D_8005E388
 * every frame, so it is probably reached through that pointer. It submits the
 * frame: with D_8005E278 == 1 it puts the active context's DISPENV/DRAWENV and
 * draws its four ordering tables, then always draws the frame ordering table. */
INCLUDE_ASM("build/asm/nonmatchings/modules/game_loop", PresentFrame);

void SceneEntryHook(s32 arg0) {
}

/*
 * Scene-selection handshake between overlay callbacks and the EXE dispatcher.
 * Requesting a change records the two dispatch IDs and ends the active update
 * loop; main performs the next scene's setup after that loop returns.
 */

/* Shared with main and the update loop; the tentative definitions give the
 * original GP-relative accesses. */
s32 gPreviousSceneId;

void RequestSceneChange(s32 sceneId) {
    s32 previousSceneId;

    /* Snapshot before replacing the ID, even for a repeated/same-ID request. */
    previousSceneId = gCurrentSceneId;
    gSceneUpdateLoopActive = 0;
    gCurrentSceneId = sceneId;
    gPreviousSceneId = previousSceneId;
}

s32 GetPreviousSceneId(void) {
    return gPreviousSceneId;
}

s32 GetCurrentSceneId(void) {
    return gCurrentSceneId;
}

s32 D_8005E280;

void SetVal8005E280(s32 arg0) {
    D_8005E280 = arg0;
}

/*
 * Shared update-timing mode, read by input repeat, CD-volume decay and the
 * feedback fade. See update_timing.h for each consumer's interpretation.
 */

/* Tentative definition retains GP-relative access to the existing data byte.
 * The initial value (2) remains in the original .sdata, not a C initializer. */
u8 gUpdateTimingMode;

void SetUpdateTimingMode(u8 mode) {
    gUpdateTimingMode = mode;
}

u8 GetUpdateTimingMode(void) {
    return gUpdateTimingMode;
}

s32 D_8005E278;

void SetVal8005E278(s32 arg0) {
    D_8005E278 = arg0;
}

void SetVal8005E27C(s32 arg0) {
    D_8005E27C = arg0;
}

/*
 * Upper-bound checks for the active render context's libgs packet cursor.
 */

s32 ReservePacketWorkBytes(s32 byteCount) {
    s32 bufferEnd;
    s32 cursor;

    bufferEnd = gActiveRenderContext->packetWorkBase + gActiveRenderContext->packetWorkCapacityBytes;
    cursor = (s32)GsGetWorkBase();
    if ((u32)bufferEnd < (u32)(cursor + byteCount)) {
        /* The assertion helper breaks and returns nonzero: overflow normally
         * stays here. Preserve the error call if execution ever continues. */
        do {
        } while (DebugBreakAndReturnTrue() != 0);
        SystemError(0x50, 0);
    }
    GsSetWorkBase((PACKET *)(cursor + byteCount));
    return cursor;
}

void SetPacketWorkCursor(s32 cursor) {
    if ((u32)(gActiveRenderContext->packetWorkBase + gActiveRenderContext->packetWorkCapacityBytes) < (u32)cursor) {
        do {
        } while (DebugBreakAndReturnTrue() != 0);
    }
    GsSetWorkBase((PACKET *)cursor);
}

/* Dead code, not decompiled: nothing in the shipped game calls it or takes
 * its address. It returns GetAssetScratchEnd() - (D_8007AFF4 + 0x83000): the
 * scratch capacity above a fixed 0x83000-byte reservation, where
 * GetAssetScratchCapacity subtracts 0x49000 plus the loaded byte count. */
INCLUDE_ASM("build/asm/nonmatchings/modules/game_loop", GetFixedScratchCapacity);

/*
 * Asset-loading scratch bounds. Start calls Capacity, all three read the end
 * boundary D_8005E3B0, and ovl_11's loader tests capacity before reading into
 * Start.
 */

/* The graphics-heap initializers set this end boundary; the tentative
 * definition gives the original GP-relative accesses. */
s32 gGraphicsHeapBase;

s32 GetAssetScratchCapacity(void) {
    s32 reservationAdjustment;
    s32 remainingBytes;

    /* The lower boundary is D_8007AFF4 + 0x49000 + D_8005E328:
     * boot sets the first address, the loader records the variable byte count.
     * Keep the staged differences around D_8001009C: algebraically cancelling
     * that reference changes the original load/arithmetic sequence. */
    reservationAdjustment = -0x49000;
    remainingBytes = gGraphicsHeapBase - D_8001009C;
    remainingBytes = (remainingBytes + reservationAdjustment) - D_8005E328;
    return remainingBytes - (D_8007AFF4 - D_8001009C);
}

s32 GetAssetScratchStart(void) {
    return gGraphicsHeapBase - GetAssetScratchCapacity();
}

s32 GetAssetScratchEnd(void) {
    return gGraphicsHeapBase;
}

/* Globals accessed GP-relatively (this TU defines them) */
s32 D_8005E3BC;

void ResetActiveRenderContext(void) {
    GsSetWorkBase((PACKET *)gActiveRenderContext->packetWorkBase);
    __asm__ volatile("" ::: "memory");
    {
        s32 temp = gActiveRenderContextIndex * 0x3000;
        temp += 0x38E50;
        D_8005E3BC = gGraphicsHeapBase + temp;
    }
    ClearOTagR(gFrameOrderingTable, 2);

    if (gActiveRenderContext->field_D8) {
        ClearOTagR(gActiveRenderContext->field_D8, gActiveRenderContext->field_EC);
    }
    if (gActiveRenderContext->field_DC) {
        ClearOTagR(gActiveRenderContext->field_DC, gActiveRenderContext->field_F0);
    }
    if (gActiveRenderContext->field_E0) {
        ClearOTagR(gActiveRenderContext->field_E0, gActiveRenderContext->field_F4);
    }
    if (gActiveRenderContext->field_E4) {
        ClearOTagR(gActiveRenderContext->field_E4, gActiveRenderContext->field_F8);
    }
    ClearOTagR(gActiveRenderContext->field_120, 0x800);
    gActiveRenderContext->field_118 = gActiveRenderContext->field_124;
}

/* Globals accessed GP-relatively (this TU defines them) */

void ClearActiveOrderingTables(void) {
    if (gActiveRenderContext->field_D8) {
        ClearOTagR(gActiveRenderContext->field_D8, gActiveRenderContext->field_EC);
    }
    if (gActiveRenderContext->field_DC) {
        ClearOTagR(gActiveRenderContext->field_DC, gActiveRenderContext->field_F0);
    }
    if (gActiveRenderContext->field_E0) {
        ClearOTagR(gActiveRenderContext->field_E0, gActiveRenderContext->field_F4);
    }
    if (gActiveRenderContext->field_E4) {
        ClearOTagR(gActiveRenderContext->field_E4, gActiveRenderContext->field_F8);
    }
    ClearOTagR(gActiveRenderContext->field_120, 0x800);
    gActiveRenderContext->field_118 = gActiveRenderContext->field_124;
}

void SelectRenderConfiguration(s32 arg0) {
    InitFunc fp;

    do {
    } while (DrawSync(1));
    SetGeomOffset(0, 0);
    SetGeomScreen(0x3E8);
    GsSetOffset(0, 0);
    GsSetDrawBuffOffset();
    fp = D_80010000[arg0];
    fp();
    ResetActiveRenderContext();
    ResetActiveRenderContext();
}

/* TU-owned globals (GP-relative in the target) */
s32 D_8005E3B8;

/*
 * InitGraphicsHeapCompact -- graphics heap carve + double-buffer /
 * ordering-table init for render configuration 0 (D_80010000[0], selected by
 * SelectRenderConfiguration(0)); its packet work areas are half the size of
 * configuration 1's.
 *
 * Near-verbatim clone of InitGraphicsHeap (matched 2026-08-11): same 0x40 frame,
 * same memset(0x801BE1B0, 0, 0x3EE50), and a first loop whose 21 register webs
 * and store partition are identical. Six deltas:
 *   1. D_8005E3BC/D_8005E3B8 = 0x801F7000, not 0x801FCFF8.
 *   2. work-area stride 0x2EE0, not 0x5EDC (spill increment + 0x128 length).
 *   3. second phase is a call to func_8001E160, not an inlined loop.
 *   4. register rotation: &D_8005E5E8 in $t8, byte pointer in $t9, pool
 *      cursors p0/p1/p2 in $s0/$s1/$s2 (see recipe for the full table).
 *   5. base is addu $a0, $t8, $t7 -- base register first.
 *   6. symbol load is the lui %hi / addiu %lo pair into $t8, then t9 = t8.
 *
 * USER-AUTHORIZED EXCEPTION: the parked-function campaign explicitly permits
 * source-policy exceptions. These allocation-forcing constructs are retained
 * from the historical attempt; they are not evidence of original source pins.
 * Shares graphics_heap.h's layout and matching helpers with InitGraphicsHeap.
 * Eight redundant bindings have been removed together; ten remain explicit.
 * Reload-created copies are a supported interpretation of the shared target
 * pattern, not an observed original pass history. An original helper macro
 * is possible, but neither that nor a C impossibility has been established.
 * See notes/asm-exception-evidence.md for the measured scope.
 * Two forcing devices beyond the twin's:
 *   - block 1 routes the spilled work-area accumulator through $v0 (rV0),
 *     hoists MATCH_COPY(rV1, rA0) next to the base, and births the 0x40 constant
 *     after the accumulator's last use -- the mirror image of the twin's
 *     ordering, which the plain statement swap cannot reproduce because the
 *     accumulator's register choice is an allocation decision.
 *   - block 0 materialises memset's a0/a2 into $4/$6 via non-volatile asm so
 *     the call's own expansion only births `move a1,0`, which then fills the
 *     jal-delay slot (sched2/dbr otherwise put `ori a2` there). Non-volatile,
 *     not volatile: the pairs must stay schedulable to sit ahead of the
 *     callee-save store block as in the target. The tested C spellings
 *     (heap-size local, register vars) folded back into the call.
 * See notes/techniques-for-solving-parked-functions.md for the authorization
 * and byte/full-build verification, and notes/research/func_8001231C.md for
 * the historical reconstruction.
 */

#define P(x) ((s32 *)(x))

void InitGraphicsHeapCompact(void) {
    register u8 *t8 asm("$24");
    register u8 *t9 asm("$25");
    s32 s0;
    s32 s1;
    s32 s2;
    s32 s3;
    s32 s4;
    s32 s5;
    s32 s6;
    s32 s7;
    register s32 t7 asm("$15");
    register s32 rV0 asm("$2");
    register s32 rV1 asm("$3");
    register s32 *rA0 asm("$4");
    register s32 *rA1 asm("$5");
    register s32 temp_a2 asm("$6");
    register s32 *rA3 asm("$7");
    s32 * rT0;
    s32 * rT1;
    s32 * rT2;
    s32 * rT3;
    s32 * rT4;
    s32 * rT5;
    register s32 *rT6 asm("$14");
    s32 s8;
    s32 sp10;

    s7 = GFX_HEAP_BASE;
    __asm__("lui %0, 0x801B" : "=r"(rA0));
    __asm__("ori %0, %0, 0xE1B0" : "+r"(rA0));
    __asm__("lui %0, 0x3" : "=r"(temp_a2));
    __asm__("ori %0, %0, 0xEE50" : "+r"(temp_a2));
    gGraphicsHeapBase = s7;
    memset((void *)rA0, 0, temp_a2);
    rV1 = GFX_HEAP_END(GFX_HEAP_WORK_SMALL_SIZE);
    s5 = 1;
    __asm__("" : "=r"(s5) : "0"(s5));
    s4 = GFX_HEAP_ADDR(GFX_HEAP_PRIM_OFFSET);
    MATCH_LOAD_ADDRESS(t8, rV0, gRenderContexts);
    t9 = t8;
    s3 = GFX_HEAP_ADDR(GFX_HEAP_MAIN_OT_OFFSET);
    s2 = GFX_HEAP_ADDR(GFX_HEAP_OT2_OFFSET);
    s1 = GFX_HEAP_ADDR(GFX_HEAP_OT1_OFFSET);
    s0 = GFX_HEAP_ADDR(GFX_HEAP_OT0_OFFSET);
    t7 = 0;
    s6 = GFX_HEAP_PRIM_SIZE;
    s8 = 1;
    sp10 = 0;
    __asm__("" : : "m"(sp10));
    D_8005E3BC = rV1;
    D_8005E3B8 = rV1;

    do {
        temp_a2 = GFX_HEAP_WORK_OFFSET;
        rA1 = P(t8 + 0xD8);
        rA1 = P(t7 + (s32)rA1);
        __asm__("" : "=r"(rA1) : "0"(rA1));
        rA0 = P((u8 *)t8 + t7);
        MATCH_COPY(rV1, rA0);
        rA3 = P(t8 + 0xDC);
        rA3 = P(t7 + (s32)rA3);
        rT6 = P(t8 + 0xE0);
        rT6 = P(t7 + (s32)rT6);
        rV0 = sp10;
        __asm__ volatile("addiu %0, %0, -1" : "+r"(s8));
        temp_a2 = rV0 + temp_a2;
        rV0 += GFX_HEAP_WORK_SMALL_SIZE;
        sp10 = rV0;

        rV0 = 0x40;
        *rA1 = s0;
        rA0[0x3B] = rV0;
        rV0 = 6;
        P(rV1)[0x40] = rV0;
        rV0 = 0x10;
        *rA3 = s1;
        rA0[0x3C] = rV0;
        rV0 = 4;
        P(rV1)[0x41] = rV0;
        rV0 = 2;
        *rT6 = s2;
        rA0[0x3D] = rV0;

        rV0 = rV1;
        P(rV1)[0x42] = s5;
        P(rV0)[0x39] = 0;
        P(rV1)[0x3E] = 0;
        P(rV0)[0x43] = 0;
        rV0 = 0x800;
        P(rV1)[0x44] = rV0;
        rV0 = 0xB;
        rA0[0x45] = rV0;
        P(rV1)[0x48] = s3;
        s3 += 0x2000;
        s2 += 8;
        s1 += 0x40;
        s0 += 0x100;

        rV0 = rV1;
        rV1 = 6;
        t9[0x16] = (u8)s5;
        t9[0x17] = (u8)s5;
        t9[0x18] = (u8)s5;
        t9[0x19] = 0;
        t9[0x1A] = 0;
        t9[0x1B] = 0;
        P(rV0)[0x21] = rV1;

        rV1 = rV0;
        MATCH_COPY(rT0, rV0);
        MATCH_COPY(rT1, rV0);
        MATCH_COPY(rT2, rV0);
        MATCH_COPY(rT3, rV0);
        MATCH_COPY(rT4, rV0);
        MATCH_COPY(rT5, rV0);
        P(rV1)[0x24] = 0;
        P(rV0)[0x23] = 0;

        rA0 = P(*rA1);
        temp_a2 = s7 + temp_a2;
        P(rV0)[0x22] = (s32)rA0;
        rV1 = *rA1;
        MATCH_COPY(rA0, rV0);
        rV1 += 0xFC;
        rA0[0x25] = rV1;
        rV1 = 4;
        P(rV0)[0x26] = rV1;

        MATCH_COPY(rV1, rV0);
        P(rV1)[0x29] = 0;
        P(rV0)[0x28] = 0;
        rA0 = P(*rA3);
        MATCH_COPY(rA1, rV0);
        P(rV0)[0x27] = (s32)rA0;
        rV1 = *rA3;
        MATCH_COPY(rA0, rV0);
        rV1 += 0x3C;
        rA0[0x2A] = rV1;

        MATCH_COPY(rV1, rV0);
        P(rV0)[0x2B] = s5;
        P(rV1)[0x2E] = 0;
        P(rV0)[0x2D] = 0;
        rV1 = *rT6;
        rA3 = rV0;
        rA0[0x2C] = rV1;
        rV0 = *rT6;
        t7 += 0x134;
        rV0 += 4;
        rA1[0x2F] = rV0;
        rV0 = GFX_HEAP_WORK_SMALL_SIZE;
        rA3[0x30] = 0;
        rT0[0x33] = 0;
        rT1[0x32] = 0;
        rT2[0x31] = 0;
        rT3[0x34] = 0;
        rT4[0x4A] = rV0;
        rT5[0x4B] = temp_a2;
        *(s32 *)(t9 + 0x130) = s4;
        s4 += s6;
        t9 += 0x134;
    } while (s8 >= 0);

    InitRenderPacketPools();
}

/* TU-owned globals (GP-relative in the target) */

/*
 * InitGraphicsHeap -- graphics heap carve + double-buffer / ordering-table
 * init for render configuration 1 (D_80010000[1], selected by
 * SelectRenderConfiguration(1)).
 *
 * MATCHED. `make check` is green and `psx_diff_function --bytes` reports
 * byte-identical in the linked binary at the original address.
 *
 * POLICY EXCEPTION -- hard-register pinning and embedded asm, granted
 * explicitly by the user on 2026-08-11 (AGENTS.md defers the clean-source
 * policy to explicit user instruction). Allowlisted in .pi/autoloop.json.
 *
 * This is tracked debt, not a claim about the original source. A historical
 * candidate introduced pointer copies during allocation/reload (4 register
 * copies at .lreg, 18 at .greg); that supports a compiler-generated-copy
 * interpretation, but is not the original compiler's pass history. An
 * original helper macro is also possible, not established.
 *
 * Shared heap-layout names and matching helpers live in graphics_heap.h.
 * Eight redundant bindings have been removed together; ten remain explicit
 * here. The helpers preserve existing asm, not grant a new exception. See
 * notes/asm-exception-evidence.md for the bounded tests and remaining debt,
 * and notes/research/func_80012598.md for the historical investigation.
 *
 * WHAT THE FUNCTION DOES
 *
 * D_8005E5E8[2] is the double-buffered render context, stride 0x134
 * (see include/globals_override.h). Field map, from the store set plus the
 * sibling users:
 *
 *   0x000  DRAWENV draw      main calls SetDefDrawEnv on +0x000/+0x134
 *   0x016  draw.dtd/dfe/isbg = 1, draw.r0/g0/b0 = 0    (the six sb)
 *   0x05C  DISPENV disp      main: SetDefDispEnv on +0x05C/+0x190
 *   0x084  GsOT ot[4]        libgs GsOT is exactly 0x14 bytes
 *                            (length/org/offset/point/tag);
 *                            tag == org + ((1 << length) - 1)
 *   0x0D8  GsOT_TAG *org[4]  = { p0, p1, p2, NULL }
 *   0x0EC  u_long   n[4]     = { 0x40, 0x10, 2, 0 }     (n == 1 << bits)
 *   0x100  u_long   bits[4]  = { 6, 4, 1, 0 }
 *   0x110  big OT: length 0x800 (== 1 << 0xB), bits 0xB
 *   0x118  prim cursor, 0x11C prim length 0x17700,
 *          0x120 OT base, 0x124 prim base      (all four also written by the
 *                                               matched sibling func_8001E160)
 *   0x128  work-area length 0x5EDC, 0x12C work-area base
 *          (packetWorkCapacityBytes / packetWorkBase in the shared view;
 *           ResetActiveRenderContext: GsSetWorkBase(...->packetWorkBase))
 *   0x130  VRAM staging base (CopyRenderContextBackup memcpy target)
 *
 * Every pool is carved out of the single 0x3EE50-byte block at 0x801BE1B0 that
 * the leading memset clears; 0x33090 + 2*0x5EDC == 0x3EE48, so the carve ends
 * eight bytes before the end of that block:
 *   +0x00000  p0  OT tags, 0x40 entries per buffer   (cursor stride 0x100)
 *   +0x00200  p1  OT tags, 0x10 entries per buffer   (cursor stride 0x040)
 *   +0x00280  p2  OT tags, 2 entries per buffer      (cursor stride 0x008)
 *   +0x00290  big OT, 0x800 entries per buffer       (cursor stride 0x2000)
 *   +0x04290  primitive buffers, 0x17700 each
 *   +0x33090  work areas, 0x5EDC each
 *
 * Loop 2 is the same source as the matched sibling func_8001E160 and is plain
 * C: it rewrites 0x110/0x114/0x120, adds 0x11C/0x124/0x118, and clears both
 * primitive buffers. Keep the two in the same shape -- writing it with
 * decomposed address temporaries instead of E160's single expressions commutes
 * two `addu` operands.
 *
 * NAMING: rV0/rV1/rA0/rA1/rA3/rT0..rT6 are the target's own register roles for
 * the struct base address. The target multiplexes each register over two or
 * three sequential roles (`$v0` alternates between holding the base and being
 * scratch for `li`), which is why they read as registers rather than as
 * variables; s0..s7/t7..t9 are the loop-carried pool cursors and constants.
 */

void InitGraphicsHeap(void) {
    register u8 *s0 asm("$16");
    register u8 *s1 asm("$17");
    s32 s2;
    s32 s3;
    s32 s4;
    s32 s5;
    s32 s6;
    s32 s7;
    register s32 t7 asm("$15");
    s32 t8;
    s32 t9;
    register s32 rV0 asm("$2");
    register s32 rV1 asm("$3");
    register s32 *rA0 asm("$4");
    register s32 *rA1 asm("$5");
    register s32 temp_a2 asm("$6");
    register s32 *rA3 asm("$7");
    s32 * rT0;
    s32 * rT1;
    s32 * rT2;
    s32 * rT3;
    s32 * rT4;
    s32 * rT5;
    register s32 *rT6 asm("$14");
    s32 s8;
    s32 sp10;
    s32 i;
    s32 j;
    s32 temp_s3;

    s7 = GFX_HEAP_BASE;
    gGraphicsHeapBase = s7;
    memset((void *)GFX_HEAP_BASE, 0, GFX_HEAP_CLEAR_SIZE);
    rV1 = GFX_HEAP_END(GFX_HEAP_WORK_LARGE_SIZE);
    s5 = 1;
    __asm__("" : "=r"(s5) : "0"(s5));
    s4 = GFX_HEAP_ADDR(GFX_HEAP_PRIM_OFFSET);
    MATCH_LOAD_ADDRESS(s0, rV0, gRenderContexts);
    s1 = s0;
    s3 = GFX_HEAP_ADDR(GFX_HEAP_MAIN_OT_OFFSET);
    t9 = GFX_HEAP_ADDR(GFX_HEAP_OT2_OFFSET);
    t8 = GFX_HEAP_ADDR(GFX_HEAP_OT1_OFFSET);
    s2 = GFX_HEAP_ADDR(GFX_HEAP_OT0_OFFSET);
    t7 = 0;
    s6 = GFX_HEAP_PRIM_SIZE;
    s8 = 1;
    sp10 = 0;
    __asm__("" : : "m"(sp10));
    D_8005E3BC = rV1;
    D_8005E3B8 = rV1;

    do {
        temp_a2 = GFX_HEAP_WORK_OFFSET;
        rA1 = P(s0 + 0xD8);
        rA1 = P(t7 + (s32)rA1);
        __asm__("" : "=r"(rA1) : "0"(rA1));
        rA0 = P((u8 *)s0 + t7);
        rV0 = 0x40;
        rA3 = P(s0 + 0xDC);
        rA3 = P(t7 + (s32)rA3);
        rT6 = P(s0 + 0xE0);
        rT6 = P(t7 + (s32)rT6);
        rV1 = sp10;
        __asm__ volatile("addiu %0, %0, -1" : "+r"(s8));
        temp_a2 = rV1 + temp_a2;
        rV1 += GFX_HEAP_WORK_LARGE_SIZE;
        sp10 = rV1;

        MATCH_COPY(rV1, rA0);
        *rA1 = s2;
        rA0[0x3B] = rV0;
        rV0 = 6;
        P(rV1)[0x40] = rV0;
        rV0 = 0x10;
        *rA3 = t8;
        rA0[0x3C] = rV0;
        rV0 = 4;
        P(rV1)[0x41] = rV0;
        rV0 = 2;
        *rT6 = t9;
        rA0[0x3D] = rV0;

        rV0 = rV1;
        P(rV1)[0x42] = s5;
        P(rV0)[0x39] = 0;
        P(rV1)[0x3E] = 0;
        P(rV0)[0x43] = 0;
        rV0 = 0x800;
        P(rV1)[0x44] = rV0;
        rV0 = 0xB;
        rA0[0x45] = rV0;
        P(rV1)[0x48] = s3;
        s3 += 0x2000;
        t9 += 8;
        t8 += 0x40;
        s2 += 0x100;

        rV0 = rV1;
        rV1 = 6;
        s1[0x16] = (u8)s5;
        s1[0x17] = (u8)s5;
        s1[0x18] = (u8)s5;
        s1[0x19] = 0;
        s1[0x1A] = 0;
        s1[0x1B] = 0;
        P(rV0)[0x21] = rV1;

        rV1 = rV0;
        MATCH_COPY(rT0, rV0);
        MATCH_COPY(rT1, rV0);
        MATCH_COPY(rT2, rV0);
        MATCH_COPY(rT3, rV0);
        MATCH_COPY(rT4, rV0);
        MATCH_COPY(rT5, rV0);
        P(rV1)[0x24] = 0;
        P(rV0)[0x23] = 0;

        rA0 = P(*rA1);
        temp_a2 = s7 + temp_a2;
        P(rV0)[0x22] = (s32)rA0;
        rV1 = *rA1;
        MATCH_COPY(rA0, rV0);
        rV1 += 0xFC;
        rA0[0x25] = rV1;
        rV1 = 4;
        P(rV0)[0x26] = rV1;

        MATCH_COPY(rV1, rV0);
        P(rV1)[0x29] = 0;
        P(rV0)[0x28] = 0;
        rA0 = P(*rA3);
        MATCH_COPY(rA1, rV0);
        P(rV0)[0x27] = (s32)rA0;
        rV1 = *rA3;
        MATCH_COPY(rA0, rV0);
        rV1 += 0x3C;
        rA0[0x2A] = rV1;

        MATCH_COPY(rV1, rV0);
        P(rV0)[0x2B] = s5;
        P(rV1)[0x2E] = 0;
        P(rV0)[0x2D] = 0;
        rV1 = *rT6;
        rA3 = rV0;
        rA0[0x2C] = rV1;
        rV0 = *rT6;
        t7 += 0x134;
        rV0 += 4;
        rA1[0x2F] = rV0;
        rV0 = GFX_HEAP_WORK_LARGE_SIZE;
        rA3[0x30] = 0;
        rT0[0x33] = 0;
        rT1[0x32] = 0;
        rT2[0x31] = 0;
        rT3[0x34] = 0;
        rT4[0x4A] = rV0;
        rT5[0x4B] = temp_a2;
        *(s32 *)(s1 + 0x130) = s4;
        s4 += s6;
        s1 += 0x134;
    } while (s8 >= 0);

    temp_s3 = gGraphicsHeapBase;
    j = 0;
    s2 = (GFX_HEAP_PRIM_SIZE - 1);
    do {
        s32 *base2;
        base2 = (s32 *)((u8 *)&gRenderContexts + (j * 0x134));
        base2[0x44] = 0x800;
        base2[0x45] = 0xB;
        base2[0x48] = temp_s3 + GFX_HEAP_MAIN_OT_OFFSET + (j << 13);
        base2[0x47] = GFX_HEAP_PRIM_SIZE;
        base2[0x49] = temp_s3 + GFX_HEAP_PRIM_OFFSET + (j * GFX_HEAP_PRIM_SIZE);
        ClearOTagR((unsigned long *)base2[0x48], 0x800);
        i = 0;
        do {
            ((u8 *)base2[0x49])[i] = 0;
            i++;
        } while (i <= s2);
        base2[0x46] = base2[0x49];
        j++;
    } while (j < 2);
}

/* TU-owned globals (GP-relative in target) */
s32 D_8005E380;

void CopyRenderContextBackup(s32 arg0, s32 arg1) {
    s32 var_a1;
    s32 var_a2;

    if (arg0 == 0) {
        var_a1 = 0;
        if (arg1 == 0) {
            var_a1 = 1;
            var_a2 = 0x10000;
        } else {
            var_a2 = *(s32 *)((char *)gActiveRenderContext + 0x11C) * 2;
        }
        memcpy(gRenderContexts[var_a1].field_130, (void *)D_8001009C, var_a2);
        D_8005E398 = 1;
        D_8005E380 = arg1;
        return;
    }
    var_a1 = 0;
    if (D_8005E380 == 0) {
        var_a1 = 1;
        var_a2 = 0x10000;
    } else {
        var_a2 = *(s32 *)((char *)gActiveRenderContext + 0x11C) * 2;
    }
    memcpy((void *)D_8001009C, gRenderContexts[var_a1].field_130, var_a2);
    D_8005E398 = 0;
}
