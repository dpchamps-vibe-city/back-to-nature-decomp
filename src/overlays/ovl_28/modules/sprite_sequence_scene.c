/*
 * Sprite-sequence scene, the whole ovl_28 translation unit (27 functions).
 *
 * Seven phases load sprites and sound, run a bytecode-driven vertical sprite
 * sequence alongside timed sprite presentations, fade out and select the next
 * scene. Private callback slots join the two independent update paths.
 *
 * Scrolling positions/steps are Q12 pixels; presentation rectangles use
 * integer pixels. Durations count active calls, not elapsed milliseconds.
 * Original data, handler tables and GPU packet storage remain in the overlay.
 * See ovl28_sprite_sequence_scene.h and notes/file-groupings.md.
 */

#include "common.h"
#include "ovl28_sprite_sequence_scene.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libsnd.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "screen_primitives.h"
#include "scene_selection.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "packet_work.h"
#include "message_window.h"
#include "sprite_source.h"
#include "random.h"
#include "sprite_upload.h"
#include "sprite_render.h"

void func_80017A48(u32 arg0);
u32 func_80017A64(void);

void Ovl28UpdateSpriteSequenceScene(void) {
    s32 *handlers;
    u32 savedDrawMode;

    savedDrawMode = func_80017A64();
    func_80017A48(3U);
    handlers = &gOvl28SpriteSceneHandlers;
    /* The table-as-argument dispatch ABI is shared with the other scenes;
     * these handlers do not read the argument. No phase bounds check. */
    ((void (*)(s32 *))handlers[gScenePhase])(handlers);
    func_80017A48(savedDrawMode);
}

s32 Ovl28InitScrollingSpriteSequence(void);
void Ovl28InitSpritePresentation(void);

/* Reset private paths, not the shared gScenePhase. The phase handlers keep
 * their existing game-state-base view of that word at byte offset 0x4488. */
void Ovl28ResetSpriteSequenceScene(void) {
    gOvl28ScrollingSpriteHandler = (void (*)(void))Ovl28InitScrollingSpriteSequence;
    gOvl28SpriteAssetLoadPhase = 0;
    gOvl28SpriteSceneDisplayPhase = 0;
    gOvl28SpriteSceneTick = 0;
    gOvl28SpriteCommandIndex = 0;
    gOvl28SpriteCommandTicks = 0;
    gOvl28SpriteCommandDelay = 0;
    gOvl28SpriteScrollEnabled = 1;
    gOvl28SpriteScrollStepQ12 = 0x800;
    gOvl28SpriteSequenceComplete = 0;
    gOvl28PresentationHandler = Ovl28InitSpritePresentation;
    gOvl28PresentationSetIndex = 0;
    gOvl28PresentationEntryIndex = 0;
    gOvl28PresentationEnabled = 0;
    gOvl28PresentationPhase = -1;
    gOvl28PresentationId = 0;
    gOvl28PresentationX = 0;
    gOvl28PresentationY = 0;
}

void Ovl28ResetSpriteSequenceScene (void);
void Ovl28StartSequenceSpriteRead (void);

void Ovl28BeginSpriteSequenceScene(void) {
    s32 *base;

    Ovl28ResetSpriteSequenceScene();
    BeginScreenFadeIncrease(0, 0, 2);
    Ovl28StartSequenceSpriteRead();
    ResetMessageWindow();
    base = (s32 *)&D_8006C838;
    base[0x1122] = base[0x1122] + 1;
}

s32 Ovl28PollSequenceSpriteRead (void);

void Ovl28PollInitialSpriteAssets(void) {
    s32 *base;

    if ((Ovl28PollSequenceSpriteRead() != 0) && (IsScreenFadeComplete() == 1)) {
        gOvl28SpriteAssetLoadPhase = 0;
        base = (s32 *)&D_8006C838;
        base[0x1122] = base[0x1122] + 1;
    }
}

void Ovl28QueueSpriteSequenceSoundLoads (void);
void Ovl28StartBackdropSpriteRead (void);
s32 Ovl28PollBackdropSpriteRead (void);

void Ovl28PrepareSpriteSequenceScene(void) {
    s32 *base;

    switch (gOvl28SpriteAssetLoadPhase) {
    case 0x0:
        gOvl28SpriteAssetLoadPhase = 0xA;
        return;
    case 0xA:
        Ovl28QueueSpriteSequenceSoundLoads();
        gOvl28SpriteAssetLoadPhase = (u16) gOvl28SpriteAssetLoadPhase + 1;
        return;
    case 0xB:
        UpdateMasterVolumeAndSoundTransfer();
        if (IsSoundTransferComplete() != 0) {
            BeginMasterVolumeFadeIn(0xA);
            gOvl28SpriteAssetLoadPhase = (u16) gOvl28SpriteAssetLoadPhase + 1;
        }
        return;
    case 0xC:
        if (GetMasterVolumeRampState() == 0) {
            gOvl28SpriteAssetLoadPhase = 0x14;
            return;
        }
        break;
    case 0x14:
        Ovl28StartBackdropSpriteRead();
        gOvl28SpriteAssetLoadPhase = (u16) gOvl28SpriteAssetLoadPhase + 1;
        return;
    case 0x15:
        if (Ovl28PollBackdropSpriteRead() != 0) {
            gOvl28SpriteAssetLoadPhase = 0xFF;
            return;
        }
        break;
    case 0xFF:
        gOvl28SpriteAssetLoadPhase = 0;
        OpenSoundSequencesAndStart();
        PlaySoundSequenceWithReverb(0, 0, 0x23, 0x7F);
        base = (s32 *)&D_8006C838;
        base[0x1122] = base[0x1122] + 1;
        break;
    }
}

void Ovl28RunSpriteSequenceScene(void) {
    s32 *base;

    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x18, (s32) gOvl28BackdropSpriteSource, 0x1F, 0, 0, -7);
    DrawScreenTileRect((u32 *)(gActiveRenderContext->field_D8 + 0x14), 0, 0, *(s16 *)&HWD0, 0x8C, 0, 1);
    gOvl28SpriteSceneTick = (gOvl28SpriteSceneTick + 1) % 108000;
    switch (gOvl28SpriteSceneDisplayPhase) {
    case 0x0:
        BeginScreenFadeDecrease(0xA, 0, 2);
        gOvl28SpriteSceneDisplayPhase = (u16) gOvl28SpriteSceneDisplayPhase + 1;
        return;
    case 0x1:
        if (IsScreenFadeComplete() == 1) {
            gOvl28SpriteSceneDisplayPhase = (u16) gOvl28SpriteSceneDisplayPhase + 1;
        }
        return;
    case 0x2:
        gOvl28ScrollingSpriteHandler();
        gOvl28PresentationHandler();
        if (gOvl28SpriteSequenceComplete == 1) {
            BeginScreenFadeIncrease(0xA, 0, 2);
            gOvl28SpriteSceneDisplayPhase = (u16) gOvl28SpriteSceneDisplayPhase + 1;
        }
        break;
    case 0x3:
        if (IsScreenFadeComplete() == 1) {
            gOvl28SpriteSceneDisplayPhase = 0xFF;
            return;
        }
        break;
    case 0xFF:
        base = (s32 *)&D_8006C838;
        base[0x1122] = base[0x1122] + 1;
        break;
    }
}

s32 Ovl28BeginSpriteSequenceExit(void) {
    s32 *base;
    s32 v;

    BeginScreenFadeIncrease(10, 0, 2);
    BeginMasterVolumeFadeOut(10);
    base = (s32 *)&D_8006C838;
    v = base[0x4488 >> 2] + 1;
    base[0x4488 >> 2] = v;
    return v;
}

s32 Ovl28PollSpriteSequenceExit(void) {
    s32 *base;
    u8 x;
    s32 ret;

    x = (IsScreenFadeComplete() ^ 1) == 0;
    if (GetMasterVolumeRampState() == 0) {
        x = (x + 1) & 0xFF;
    }
    ret = 2;
    if (x == 2) {
        StopSoundSequence(0);
        OpenSoundSequencesAndStart();
        RewindSoundLoadSlots(2, 0);
        base = (s32 *)&D_8006C838;
        ret = base[0x4488 >> 2] + 1;
        base[0x4488 >> 2] = ret;
    }
    return ret;
}

void Ovl28FinishSpriteSequenceScene(void) {
    char *base;
    char *hi;

    base = (char *)&D_8006C838;
    *(s32 *)(base + 0x4488) = 0;
    hi = base + 0x8000;
    if (*(u16 *)(hi + 0x67A0) >= 10) {
        RequestSceneChange(0xD);
    } else {
        RequestSceneChange(0x14);
    }
}

void Ovl28ClearScrollingSprites(void);
void Ovl28UpdateScrollingSpriteSequence(void);

s32 Ovl28InitScrollingSpriteSequence(void) {
    Ovl28ClearScrollingSprites();
    gOvl28ScrollingSpriteHandler = Ovl28UpdateScrollingSpriteSequence;
    gOvl28SpriteCommandIndex = 0;
    gOvl28SpriteCommandTicks = 0;
    gOvl28SpriteCommandDelay = 0;
    gOvl28SpriteScrollEnabled = 1;
    gOvl28SpriteScrollStepQ12 = 0x800;
    gOvl28SpriteSequenceComplete = 0;
    return 0x800;
}

void Ovl28DrawScrollingSprites(void);
void Ovl28StepScrollingSprites(void);
void Ovl28QueueScrollingSprite(s16 arg0);

/* F0: fade sound; F1: freeze scrolling; F2/F3: enable/disable presentations;
 * FF: mark complete. Control bytes chain in this call. In particular FF does
 * not return: the original continues to the next non-control byte. Dn and En
 * extend the base interval by n*14 pixels of travel or n*30 calls. */
void Ovl28UpdateScrollingSpriteSequence(void) {
    s32 commandClass;
    s32 baseDelay;
    u8 command;

    gOvl28SpriteCommandTicks = (u16) gOvl28SpriteCommandTicks + 1;
    Ovl28DrawScrollingSprites();
    if (gOvl28SpriteScrollEnabled == 1) {
        Ovl28StepScrollingSprites();
    }
    if (gOvl28SpriteCommandDelay > 0) {
        if (gOvl28SpriteCommandTicks >= gOvl28SpriteCommandDelay) {
            gOvl28SpriteCommandDelay = 0;
            goto read_command;
        }
    } else {
read_command:
        command = gOvl28SpriteCommands[gOvl28SpriteCommandIndex];
        baseDelay = 0x10000 / gOvl28SpriteScrollStepQ12;
        commandClass = command & 0xF0;
        gOvl28SpriteCommandDelay = (s16) baseDelay;
        gOvl28SpriteCommandTicks = 0;
        switch (commandClass) {
        case 0xF0:
            if (command == 0xFF) {
                gOvl28SpriteSequenceComplete = 1;
            } else if (command == 0xF3) {
                gOvl28PresentationEnabled = 0;
            } else if (command == 0xF2) {
                gOvl28PresentationEnabled = 1;
            } else if (command == 0xF1) {
                gOvl28SpriteScrollEnabled = 0;
            } else if (command == commandClass) {
                BeginMasterVolumeFadeOut(0x3C);
            }
            gOvl28SpriteCommandIndex = (u16) gOvl28SpriteCommandIndex + 1;
            goto read_command;
        case 0xE0:
            gOvl28SpriteCommandDelay = baseDelay + (command & 0xF) * 0x1E;
            break;
        case 0xD0:
            gOvl28SpriteCommandDelay = baseDelay + (0xE000 / (s32) gOvl28SpriteScrollStepQ12) * (command & 0xF);
            break;
        default:
            Ovl28QueueScrollingSprite((s16) command);
            break;
        }
        gOvl28SpriteCommandIndex = (u16) gOvl28SpriteCommandIndex + 1;
    }
}

void Ovl28UpdateSpritePresentation(void);

void Ovl28InitSpritePresentation(void) {
    char *base;
    char *hi;
    u16 v;

    gOvl28PresentationHandler = Ovl28UpdateSpritePresentation;
    gOvl28PresentationPhase = -1;
    gOvl28PresentationId = 0;
    gOvl28PresentationX = 0;
    gOvl28PresentationY = 0;
    gOvl28PresentationEnabled = 0;
    base = (char *)&D_8006C838;
    hi = base + 0x8000;
    gOvl28PresentationEntryIndex = 0;
    v = *(u16 *)(hi + 0x67A0);
    if (v >= 10) {
        gOvl28PresentationSetIndex = 10;
    } else {
        gOvl28PresentationSetIndex = v;
    }
}

s32 func_8001AD6C(s32 arg0);
void func_8001ADD8(u32 *arg0, u16 arg1, s16 arg2, s16 arg3);
void Ovl28DrawSequenceGradientRect(u_long *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s16 arg9, u8 arg10);
/* The color-ramp updater uses only private state; it takes no rectangle argument. */
void Ovl28UpdateSpriteColorRamp(void);
void Ovl28BeginSpriteColorRampDown(s16 arg0, s16 arg1);
void Ovl28BeginSpriteColorRampUp(s16 arg0, s16 arg1);
s32 Ovl28IsSpriteColorRampComplete(void);

/* -1 selects an ID, -2 waits for its resource, -3 ramps down, positive
 * values hold for that many calls, and 0 ramps up before selecting again.
 * The row and entry indices are not range-checked by this original path. */
void Ovl28UpdateSpritePresentation(void) {
    u32 *ot;
    s32 column;

    ot = (u32 *)gActiveRenderContext->field_D8 + 4;
    if (gOvl28PresentationEnabled == 1) {
        switch (gOvl28PresentationPhase) {
        case -1:
            if (gOvl28SpriteScrollEnabled == 1) {
                column = gOvl28PresentationEntryIndex;
                gOvl28PresentationEntryIndex = (u16)gOvl28PresentationEntryIndex + 1;
                gOvl28PresentationId = gOvl28PresentationIds[gOvl28PresentationSetIndex][column];
                gOvl28PresentationX = Rand((u16)(*(u16 *)&HWD0 - 80));
                gOvl28PresentationY = 144;
                gOvl28PresentationPhase = -2;
            }
            break;
        case -2:
            if (func_8001AD6C((u16)gOvl28PresentationId) == 1) {
                Ovl28BeginSpriteColorRampDown(gOvl28PresentationX, gOvl28PresentationY);
                gOvl28PresentationPhase = -3;
            }
            break;
        case -3:
            Ovl28UpdateSpriteColorRamp();
            func_8001ADD8(ot, (u16)gOvl28PresentationId, gOvl28PresentationX, gOvl28PresentationY);
            if (Ovl28IsSpriteColorRampComplete() != 0) {
                gOvl28PresentationPhase = 60;
            }
            break;
        case 0:
            Ovl28UpdateSpriteColorRamp();
            func_8001ADD8(ot, (u16)gOvl28PresentationId, gOvl28PresentationX, gOvl28PresentationY);
            if (Ovl28IsSpriteColorRampComplete() != 0) {
                gOvl28PresentationPhase = -1;
            }
            break;
        default:
            func_8001ADD8(ot, (u16)gOvl28PresentationId, gOvl28PresentationX, gOvl28PresentationY);
            Ovl28DrawSequenceGradientRect((u_long *)gActiveRenderContext->field_D8 + 3,
                gOvl28PresentationX, gOvl28PresentationY, 80, 80,
                0x4B6496, 0x4B6496, 0x4B6496, 0x4B6496, 1, 2);
            if (--gOvl28PresentationPhase == 0) {
                Ovl28BeginSpriteColorRampUp(gOvl28PresentationX, gOvl28PresentationY);
            }
            break;
        }
    }
}

void Ovl28ClearScrollingSprites(void) {
    Ovl28ScrollingSprite *sprite;
    s32 startYQ12;
    s32 i;

    sprite = gOvl28ScrollingSprites;
    startYQ12 = VWD0 << 12;
    for (i = 0; i < 20; i++) {
        sprite->active = 0;
        sprite->frameIndex = 0;
        sprite->yQ12 = startYQ12;
        sprite += 1;
    }
}

/* A full pool silently drops the requested frame. */
void Ovl28QueueScrollingSprite(s16 frameIndex) {
    s32 i;

    i = 0;
    while (i < 0x14 && gOvl28ScrollingSprites[i].active != 0) {
        i += 1;
    }
    if (i != 0x14) {
        gOvl28ScrollingSprites[i].active = 1;
        gOvl28ScrollingSprites[i].frameIndex = frameIndex;
        gOvl28ScrollingSprites[i].yQ12 = VWD0 << 0xC;
    }
}

void Ovl28DrawScrollingSprites(void) {
    Ovl28ScrollingSprite *sprites;
    Ovl28ScrollingSprite *sprite;
    s32 i;

    sprites = gOvl28ScrollingSprites;
    for (i = 0; i < 20; i++) {
        sprite = sprites + i;
        if (sprite->active == 1) {
            DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 8, (s32)gOvl28ScrollingSpriteSource, 0,
                          (u8)sprite->frameIndex, 0, (s16)(sprite->yQ12 / 4096));
        }
    }
}

void Ovl28StepScrollingSprites(void) {
    Ovl28ScrollingSprite *sprite;
    s32 stepQ12;
    s32 active;
    s32 retirementYQ12;
    s32 i;

    active = 1;
    stepQ12 = gOvl28SpriteScrollStepQ12;
    retirementYQ12 = -0xE000;
    sprite = gOvl28ScrollingSprites;
    for (i = 0; i < 20; i++) {
        if (sprite->active == active) {
            sprite->yQ12 -= stepQ12;
            if (sprite->yQ12 <= retirementYQ12) {
                sprite->active = 0;
            }
        }
        sprite += 1;
    }
}

void Ovl28StartSequenceSpriteRead(void) {
    DrawSync(0);
    ClearOTagR((unsigned long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x3D77000, 0x6000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
}

/* Sprite headers are copied out before the shared read scratch is reused. */

s32 Ovl28PollSequenceSpriteRead(void) {
    if (CdReadArchiveBytes(0, 0x03D77000, 0x6000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) != 0) {
        UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4900));
        memcpy(&gOvl28ScrollingSpriteHeader, (void *)(gGraphicsHeapBase + 0x4290), 0x670);
        InitSpriteSourceData((SpriteSourceData *)gOvl28ScrollingSpriteSource, (SpriteDataHeader *)&gOvl28ScrollingSpriteHeader);
        SelectSpriteAnimation((ObjectState *)gOvl28ScrollingSpriteSource, 0);
        return 1;
    }
    return 0;
}

void Ovl28QueueSpriteSequenceSoundLoads(void) {
    SetSoundCommandReadBuffer(0, gGraphicsHeapBase + 0x4290);
    QueueSoundLoadCommand(0, 0);
    QueueSoundLoadCommand(0x3E8, 0);
    QueueSoundLoadCommand(0x23, 0);
}

void Ovl28StartBackdropSpriteRead(void) {
    DrawSync(0);
    ClearOTagR((unsigned long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x2000, 0x23000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
}

/* The backdrop uses a separate source/header pair but the same read scratch. */

s32 Ovl28PollBackdropSpriteRead(void) {
    if (CdReadArchiveBytes(0, 0x2000, 0x23000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) != 0) {
        UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4FAC));
        memcpy(&gOvl28BackdropSpriteHeader, (void *)(gGraphicsHeapBase + 0x4290), 0xD1C);
        InitSpriteSourceData((SpriteSourceData *)&gOvl28BackdropSpriteSource, (SpriteDataHeader *)&gOvl28BackdropSpriteHeader);
        SelectSpriteAnimation((ObjectState *)&gOvl28BackdropSpriteSource, 0x1F);
        return 1;
    }
    return 0;
}

/* Packed colors are 0xRRGGBB in top-left, top-right, bottom-left,
 * bottom-right order. Linking the tpage last makes it execute first. */
void Ovl28DrawSequenceGradientRect(u_long *ot, s16 x, s16 y, s16 width, s16 height, s32 topLeft, s32 topRight, s32 bottomLeft, s32 bottomRight, s16 semiTransparent, u8 blendMode) {
    POLY_G4 *packet;
    s32 widthPixels;
    s32 blend;
    s32 right;
    s32 bottom;

    widthPixels = width;
    blend = blendMode;
    packet = (POLY_G4 *)ReservePacketWorkBytes(0);
    setPolyG4(packet);
    setSemiTrans(packet, semiTransparent);
    right = x + widthPixels;
    bottom = y + height;
    setXY4(packet, x, y, right, y, x, bottom, right, bottom);
    setRGB0(packet, topLeft >> 16, topLeft >> 8, topLeft);
    setRGB1(packet, topRight >> 16, topRight >> 8, topRight);
    setRGB2(packet, bottomLeft >> 16, bottomLeft >> 8, bottomLeft);
    setRGB3(packet, bottomRight >> 16, bottomRight >> 8, bottomRight);
    addPrim(ot, packet);
    packet++;
    setDrawTPage((DR_TPAGE *)packet, 0, 1, getTPage(0, blend, 0, 0));
    addPrim(ot, packet);
    SetPacketWorkCursor((s32)((DR_TPAGE *)packet + 1));
}

void Ovl28UpdateSpriteColorRamp(void) {
    POLY_F4 *poly;
    DR_MODE *mode;
    u_long *ot;
    s32 tpage;
    Ovl28SpriteColorRampRect *rect;

    if (gOvl28SpriteColorRampMode != 0) {
        if (gOvl28SpriteColorRampMode == 1) {
            gOvl28SpriteColorRampLevel -= 4;
            if (gOvl28SpriteColorRampLevel <= 0) {
                gOvl28SpriteColorRampLevel = 0;
            }
        } else if (gOvl28SpriteColorRampMode == 2) {
            gOvl28SpriteColorRampLevel += 4;
            if (gOvl28SpriteColorRampLevel >= 255) {
                gOvl28SpriteColorRampLevel = 255;
            }
        }
        poly = (POLY_F4 *)gOvl28SpriteColorRampPoly;
        SetPolyF4(poly);
        setRGB0(poly,
            gOvl28SpriteColorRampLevel * 180 / 255 + 75,
            gOvl28SpriteColorRampLevel * 155 / 255 + 100,
            gOvl28SpriteColorRampLevel * 105 / 255 + 150);
        rect = &gOvl28SpriteColorRampRect;
        setXYWH(poly, rect->x, rect->y, rect->width, rect->height);
        SetSemiTrans(poly, 1);
        ot = (u_long *)gActiveRenderContext->field_D8;
        addPrim(ot + 3, poly);
        tpage = GetTPage(0, 2, 0, 0);
        mode = (DR_MODE *)gOvl28SpriteColorRampDrawMode;
        SetDrawMode(mode, 0, 1, tpage, NULL);
        addPrim(ot + 3, mode);
        if (gOvl28SpriteColorRampLevel == 0 || gOvl28SpriteColorRampLevel == 255) {
            gOvl28SpriteColorRampMode = 0;
        }

    }
}

/* Both directions cover an 80x80 pixel rectangle. Update changes the
 * level by four, clamps at 0/255 and emits the final endpoint packet. */
void Ovl28BeginSpriteColorRampDown(s16 x, s16 y) {
    gOvl28SpriteColorRampMode = 1;
    gOvl28SpriteColorRampLevel = 0xFF;
    gOvl28SpriteColorRampRect.x = x;
    gOvl28SpriteColorRampRect.y = y;
    gOvl28SpriteColorRampRect.width = 0x50;
    gOvl28SpriteColorRampRect.height = 0x50;
}

void Ovl28BeginSpriteColorRampUp(s16 x, s16 y) {
    gOvl28SpriteColorRampMode = 2;
    gOvl28SpriteColorRampLevel = 0;
    gOvl28SpriteColorRampRect.x = x;
    gOvl28SpriteColorRampRect.y = y;
    gOvl28SpriteColorRampRect.width = 0x50;
    gOvl28SpriteColorRampRect.height = 0x50;
}

s32 Ovl28IsSpriteColorRampComplete(void) {
    return gOvl28SpriteColorRampMode == 0;
}
