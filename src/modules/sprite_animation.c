/*
 * Sprite source records, animation service, packet construction and uploads.
 *
 * Tag-0xE headers bind a 0x30-byte instance to geometry/frame tables. The
 * animation service advances that instance in update-call units; packet
 * wrappers select the SPRT/DR_MODE, POLY_FT4 or upload-backed entry path.
 * Tag-0xD image records supply raw/RLE pixels to the upload paths.
 *
 * Interfaces: sprite_source.h (state), sprite_render.h (packet builders),
 * sprite_upload.h (VRAM transfer). Source table/image storage must outlive
 * use. This is a non-reentrant service: upload scratch, render context and
 * update/entry latches are shared. Original order and external linkages are
 * retained. Membership and conservative outer boundaries are recorded in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "debughook.h"
#include "globals_override.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "packet_work.h"
#include "sprite_render.h"
#include "sprite_source.h"
#include "sprite_upload.h"
#include "debug_break.h"

POLY_FT4 *BuildSpriteEntryPackets(s32 *ot, POLY_FT4 *packets,
                       SpriteEntrySourceView *source, s16 x, s16 y,
                       u16 flags, s32 total, s32 textureBase,
                       s16 substitute, s16 substituteFrom, s16 substituteTo);
POLY_FT4 *BuildSpriteQuadPackets(u_long *ot, POLY_FT4 *packets,
                       SpriteQuadSourceView *source, u8 animation, u8 frame,
                       s16 x, s16 y, s32 arg7, s32 arg8, s32 arg9,
                       u16 arg10, s16 arg11, s16 arg12, s16 arg13, s16 arg14);
void BuildSpriteRectPackets(u_long *ot, SPRT **sprites, DR_MODE **drawModes,
                   SpriteRectSourceView *source, u8 animation, u8 frame,
                   s16 x, s16 y);

void SetSpriteSourceHeaderAndRecord(SpriteSourceData *src, s32 header, s32 field_18);

/* Validate a sprite-data header and initialize a SpriteSourceData object.
 *
 * Checks that the header pointer is 4-byte aligned and that the header tag
 * equals 0xE (the sprite-data format magic value). On failure, prints
 * "INIT ERROR\n" and loops on the debug breakpoint (func_800129E8).
 *
 * On success: zeroes the SpriteSourceData, sets field_8 to 0x1000 (capacity
 * or size hint), computes five table pointers from header offsets 0x10–0x20,
 * and calls func_80015880 to store the header pointer at field_14 and clear
 * field_18.
 *
 * The initialized object is consumed by func_800158E4 (animation advance),
 * the dispatcher chain func_80015BF0–func_80015D6C, and the renderer
 * wrappers (func_80016C08, func_800165D8, func_80016280).
 *
 * Called by func_8001B074, func_800223D4, func_800229F4, and func_80024A4C
 * (which caches the result by comparing field_14 against the requested
 * header to skip redundant initialization). */
void InitSpriteSourceData(SpriteSourceData *out, SpriteDataHeader *header) {
    s32 callerReturnAddress;

    CAPTURE_RA(&callerReturnAddress);

    /* Verify header is 4-byte aligned */
    if ((s32)header != (((u32)header >> 2) << 2)) {
        FntPrint(D_800100A0);
        do {
        } while (DebugBreakAndReturnTrue() != 0);
    }

    /* Verify header tag (expected magic value 0xE for sprite-data format) */
    if (header->tag != 0xE) {
        FntPrint(D_800100A0);
        do {
        } while (DebugBreakAndReturnTrue() != 0);
    }

    /* Zero-initialize the SpriteSourceData and set field_8 to 0x1000 */
    out->field_6 = 0;
    out->field_0 = 0;
    out->field_2 = 0;
    out->field_4 = 0;
    out->field_5 = 0;
    out->field_8 = 0x1000;
    out->field_C = 0;
    out->field_E = 0;
    out->field_10 = 0;
    out->field_12 = 0;
    out->field_14 = 0;
    out->field_18 = 0;

    /* Compute table pointers from header offsets */
    out->field_1C = (s32)((char *)header + header->offset_tex);
    out->field_20 = (s32)((char *)header + header->offset_ref);
    out->field_24 = (s32)((char *)header + header->offset_18);
    out->field_28 = (s32)((char *)header + header->offset_anim);
    out->field_2C = (s32)((char *)header + header->offset_frame);

    /* Store header pointer at field_14 and clear field_18 */
    SetSpriteSourceHeaderAndRecord(out, header, 0);
}

void AddSpriteSourceFlags(u16 *flags, s32 mask) {
    *flags |= mask;
}

/* Flag operations pass a word mask; normalize it before modifying u16 flags. */
void ClearSpriteSourceFlags(u16 *flags, s32 mask) {
    mask &= 0xFFFF;
    *flags &= ~mask;
}

void SelectSpriteAnimation(ObjectState *obj, u8 animation) {
    u16 retainedFlags = obj->field_2;
    obj->field_4 = animation;
    obj->field_5 = 0;
    obj->field_6 = 0;
    obj->field_2 = retainedFlags & 0xFCFF;
}

void SelectSpriteAnimationFrame(ObjectState *obj, u8 frame) {
    obj->field_5 = frame;
    obj->field_6 = 0;
}

void SetSpriteSourceTextureOffsets(Struct_800154CC *source, s16 uOffset, s16 vOffset,
                   s16 paletteXOffset, s16 paletteYOffset) {
    source->field_C = uOffset;
    source->field_E = vOffset;
    source->field_10 = paletteXOffset;
    source->field_12 = paletteYOffset;
}

/* Store the sprite-data header pointer at SpriteSourceData field_14 and
 * write a value to field_18 (reserved). Only called by func_80015704
 * during sprite-source initialization. */
void SetSpriteSourceHeaderAndRecord(SpriteSourceData *src, s32 header, s32 field_18) {
    src->field_14 = header;
    src->field_18 = field_18;
}

void func_8001588C(void) {
}

void SetSpriteSourceRecordAddress(SomeStruct *source, s32 recordAddress) {
    source->field_0x18 = recordAddress;
}

/* Query one bit of the current animation frame's control byte.
 *
 * Reads the animation index table (field_28) and frame data table
 * (field_2C) from the SpriteSourceData object, exactly as the sibling
 * func_800158E4 does: field_4 indexes into the animation table to find a
 * {count, offset} pair, field_5 indexes frames within the animation, and
 * the offset + frame*0xA addresses one SpriteFrameData record. Returns
 * the control byte's high bit (bit 7), without advancing the instance.
 */

/* {count, byte offset} pair from the animation index table (field_28) */
typedef struct {
    /* 0x00 */ s16 count;   /* number of frames in this animation */
    /* 0x02 */ u16 offset;  /* byte offset into the frame data table */
} SpriteAnimRef;

/* one frame's timing and control data (10 bytes, but only first 8 accessed) */
typedef struct {
    /* 0x00 */ u16 field_0;
    /* 0x02 */ u16 field_2;
    /* 0x04 */ u16 field_4;
    /* 0x06 */ u8  duration; /* frame duration (compared against field_6) */
    /* 0x07 */ u8  control;  /* control byte: 0x80 = jump, 0xFF = end */
    /* 0x08 */ u16 field_8;
} SpriteFrameData;

u32 GetSpriteFrameControlHighBit(SpriteSourceData *src) {
    SpriteAnimRef *animRef;
    SpriteFrameData *frameData;

    animRef = (SpriteAnimRef *)(src->field_28 + (src->field_4 * sizeof(SpriteAnimRef)));
    frameData = (SpriteFrameData *)(src->field_2C + animRef->offset + (src->field_5 * 0xA));
    return frameData->control >> 7;
}

s32 gSpriteAnimationUpdateLatch;

void SetSpriteAnimationUpdateLatch(s32 value) {
    gSpriteAnimationUpdateLatch = value;
}

/* Advance sprite animation state and update frame timing.
 *
 * Reads the animation index table (field_28) and frame data table
 * (field_2C) from the SpriteSourceData object. field_4 indexes into
 * the animation table to find a {count, offset} pair; field_5 indexes
 * frames within that animation; field_6 is a frame counter that
 * increments each call and resets when the frame duration elapses.
 *
 * Control 0xFF holds the last frame and counter at half-duration. Other
 * controls with bit 7 set select frame control&0x7F in the SAME animation;
 * they do not change the animation index. At the frame-count boundary a
 * control without bit 7 also selects control&0x7F. Bit 0x100 marks a step or
 * terminal hold; bit 0x200 marks selection of a control-specified frame.
 * Both are cleared at the start of each unpaused call.
 *
 * field_0 bit 2 skips the entire service, including the update latch. Every
 * unpaused call sets that latch to 1, even if no frame changes. */

void UpdateSpriteAnimation(SpriteSourceData *src) {
    SpriteAnimRef *animRef;
    SpriteFrameData *frameData;
    u16 flags;
    u32 frameCounter;
    u16 newFlags;
    u32 halfDuration;
    u32 newFrameIdx;
    u32 controlVal;
    u32 controlByte;
    s32 animLimit;

    /* Pause guard: field_0 bit 2 set means do not advance */
    if (!(src->field_0 & 4)) {
        /* Look up the animation's {count, offset} from the animation table */
        animRef = (SpriteAnimRef *)(src->field_28 + (src->field_4 * sizeof(SpriteAnimRef)));
        /* Compute pointer into frame data: offset + frame index * 0xA */
        frameData = (SpriteFrameData *)(src->field_2C + animRef->offset + (src->field_5 * 0xA));

        /* Clear the previous call's step/control-result bits. */
        flags = src->field_2 & 0xFCFF;
        /* Increment frame counter */
        frameCounter = src->field_6 + 1;
        src->field_2 = flags;
        src->field_6 = frameCounter;

        /* Check if frame duration has elapsed (counter >= (duration+1)/2) */
        if ((s16)frameCounter >= (s32)(halfDuration = (u32)(frameData->duration + 1) >> 1)) {
            src->field_6 = 0;
            newFrameIdx = ++src->field_5;
            /* Mark the frame step. */
            newFlags = flags | 0x100;
            do {
                animLimit = animRef->count;
            } while (0);
            /* Check if we've exceeded the animation's frame count */
            if ((s32)(newFrameIdx & 0xFF) >= animLimit) {
                src->field_2 = newFlags;
                controlVal = frameData->control;
                /* At the boundary, a low-bit control selects a frame. */
                if (!(controlVal & 0x80)) {
                    src->field_5 = controlVal & 0x7F;
                    src->field_2 = (newFlags & 0xFEFF) | 0x200;
                }
            }
            controlVal = frameData->control;
            controlByte = controlVal & 0xFF;
            if (controlByte != 0) {
                if (controlByte == 0xFF) {
                    /* Hold at the terminal frame and its elapsed counter. */
                    src->field_6 = halfDuration;
                    src->field_5 = src->field_5 - 1;
                    src->field_2 = src->field_2 | 0x100;
                } else if (controlVal & 0x80) {
                    /* Select a frame within the current animation. */
                    src->field_5 = controlVal & 0x7F;
                    src->field_2 = (src->field_2 & 0xFEFF) | 0x200;
                }
            }
        }
        gSpriteAnimationUpdateLatch = 1;
    }
}

/* Read a u16 entry from the animation index table (SpriteSourceData.field_28).
 * arg1 is a signed short index multiplied by 4 (each animation-table entry is
 * 4 bytes; only the low u16 is consumed).
 */
u16 GetSpriteAnimationFrameCount(SpriteSourceData *arg0, s32 arg1) {
    return *(u16 *)(arg0->field_28 + ((s32)(arg1 << 0x10) >> 0xE));
}

/* Resolve the first entry of a selected frame; these accessors return its
 * first or second index halfword. Sentinel frame IDs 0xFFFE/0xFFFF return 0.
 * Animation and frame inputs are narrowed to their low bytes. */

u16 GetSpriteFrameEntryIndex0(SpriteSourceData *arg0, u32 arg1, u32 arg2) {
    u16 val;
    s32 ptr;
    s32 ptr_1;
    s32 ptr_2;
    s32 ptr_3;
    s32 ptr_4;

    arg1 = arg1 & 0xFF;
    ptr = arg0->field_28 + (arg1 * 4);
    val = *(u16 *)(ptr + 2);
    arg2 = arg2 & 0xFF;
    ptr_1 = arg0->field_2C + val + (arg2 * 10);
    if (*(u16 *)ptr_1 >= 0xFFFE) {
        return 0;
    }

    ptr_2 = *(u16 *)ptr_1 * 4;
    ptr_3 = arg0->field_20 + ptr_2;
    val = *(u16 *)(ptr_3 + 2);
    ptr_4 = arg0->field_24 + val;
    return *(u16 *)(ptr_4 + 0);
}

u16 GetSpriteFrameEntryIndex1(SpriteSourceData *arg0, u32 arg1, u32 arg2) {
    u16 val;
    s32 ptr;
    s32 ptr_1;
    s32 ptr_2;
    s32 ptr_3;
    s32 ptr_4;

    arg1 = arg1 & 0xFF;
    ptr = arg0->field_28 + (arg1 * 4);
    val = *(u16 *)(ptr + 2);
    arg2 = arg2 & 0xFF;
    ptr_1 = arg0->field_2C + val + (arg2 * 10);
    if (*(u16 *)ptr_1 >= 0xFFFE) {
        return 0;
    }

    ptr_2 = *(u16 *)ptr_1 * 4;
    ptr_3 = arg0->field_20 + ptr_2;
    val = *(u16 *)(ptr_3 + 2);
    ptr_4 = arg0->field_24 + val;
    return *(u16 *)(ptr_4 + 2);
}

int bcmp(const void *, const void *, int);

u16 FindSpriteTextureRecord(s32 arg0, s32 arg1, u16 arg2) {
    s32 s1 = arg1 + 4;
    s32 s2 = 0;
    s32 mask = 0xFFFFFF;
    s32 count;
    s32 offset;

    count = *(u16 *)((char *)arg1 + 2);
    offset = arg2 << 3;

    while (s2 < count) {
        s32 header = *(s32 *)s1;
        s32 temp = header & mask;
        s32 total;
        if (bcmp((char *)*(s32 **)((char *)arg0 + 0x1C) + offset,
                 (char *)s1 + 4, 8) == 0) break;
        total = ((((u32)temp + 3) >> 2) << 2) + 12;
        s1 += total;
        s2++;
    }

    if (s2 == count) {
        do {
        } while (DebugBreakAndReturnTrue() != 0);
    }

    return s2;
}

void UpdateSpriteAnimation(SpriteSourceData *src);

void DrawSpriteFrameQuads(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7, u16 arg8);

void DrawSpriteAnimationQuadsAndUpdate(s32 otAddress, SpriteSourceData *source, s32 x, s32 y) {
    DrawSpriteFrameQuads(otAddress, (s32) source, (s32) source->field_4,
                  (s32) source->field_5, (s16) (s32) x, (s16) (s32) y,
                  source->field_8, 0, 0U);
    UpdateSpriteAnimation(source);
}

void DrawSpriteFrameRects(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void DrawSpriteAnimationRectsAndUpdate(s32 otAddress, SpriteSourceData *source, s16 x, s16 y) {
    DrawSpriteFrameRects(otAddress, (s32) source, (s32) source->field_4,
                  (s32) source->field_5, (s16) (s32) x, (s16) (s32) y);
    UpdateSpriteAnimation(source);
}

POLY_FT4 *func_80015E78(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s16 arg5, s16 arg6, s32 arg7, s32 arg8, u16 arg9, s16 arg10, s16 arg11);

POLY_FT4 *func_80015CA0(s32 arg0, s32 arg1, SpriteSourceData *source, s16 x, s16 y) {
    POLY_FT4 *packets;

    packets = func_80015E78(arg0, arg1, (s32) source, (u8) (s32) source->field_4,
                  (u8) (s32) source->field_5, x, y, source->field_8, 0, 0U, -1, -1);
    UpdateSpriteAnimation(source);
    return packets;
}

void BuildSpriteRectPacketsFromAddresses(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, u8 arg5, s16 arg6, s16 arg7);

void BuildSpriteAnimationRectsAndUpdate(s32 otAddress, s32 spriteCursorAddress,
                   s32 drawModeCursorAddress, SpriteSourceData *source,
                   s16 x, s16 y) {
    BuildSpriteRectPacketsFromAddresses(otAddress, spriteCursorAddress, drawModeCursorAddress,
                  (s32) source, (u8) (s32) source->field_4,
                  (u8) (s32) source->field_5, (s16) (s32) x, (s16) (s32) y);
    UpdateSpriteAnimation(source);
}

void func_80015D6C(s32 otAddress, SpriteSourceData *source, s16 x, s16 y,
                   s32 control6, s32 control7, u16 control8) {
    DrawSpriteFrameQuads(otAddress, (s32) source, (s32) source->field_4,
                  (s32) source->field_5, x, y, control6, control7, control8);
    UpdateSpriteAnimation(source);
}

s32 func_80015DD4(s32 *arg0, SpriteSourceData *arg1, s32 arg2, s32 arg3,
                  s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    BuildSpriteEntryPackets(arg0, (POLY_FT4 *) gActiveRenderContext->field_118, arg1, (s16) arg2,
                  (s16) arg3, 0x40, arg7, arg8, -1, -1, -1);
    return 0;
}

void BuildSpriteRectPacketsFromAddresses(s32 otAddress, s32 spriteCursorAddress,
                   s32 drawModeCursorAddress, s32 sourceAddress,
                   u8 animation, u8 frame, s16 x, s16 y) {
    BuildSpriteRectPackets(otAddress, spriteCursorAddress, drawModeCursorAddress,
                            sourceAddress, animation, frame, x, y);
}

POLY_FT4 *func_80015E78(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s16 arg5, s16 arg6, s32 arg7, s32 arg8, u16 arg9, s16 arg10, s16 arg11) {
    return BuildSpriteQuadPackets(arg0, arg1, arg2, arg3 & 0xFF, arg4, arg5, arg6, arg7, arg7, arg8, arg9, arg10, arg11, -1, -1);
}

void DrawSpriteFrameRects(s32 otAddress, s32 sourceAddress, s32 animation,
                   s32 frame, s32 x, s32 y) {
    s32 animationByte;
    s32 frameByte;
    s32 xWord;
    s32 yWord;
    s32 otWord;
    s32 sourceWord;
    s32 packetCursor;

    otWord = otAddress;
    xWord = (s16)x;
    yWord = (s16)y;
    sourceWord = sourceAddress;
    animationByte = animation & 0xFF;
    frameByte = frame & 0xFF;

    packetCursor = ReservePacketWorkBytes(0);
    /* One cursor intentionally serves BOTH packet kinds: interleave the
     * sprite and draw-mode packets in the shared work buffer. */
    BuildSpriteRectPackets(otWord, &packetCursor, &packetCursor, sourceWord,
                            animationByte, frameByte, (s32) xWord, (s32) yWord);
    SetPacketWorkCursor(packetCursor);
}

void DrawSpriteFrameQuads(s32 otAddress, s32 sourceAddress, s32 animation,
                   s32 frame, s16 x, s16 y, s32 control6, s32 control7,
                   u16 control8) {
    s32 frameByte;
    s32 animationByte;

    animationByte = animation & 0xFF;
    frameByte = frame & 0xFF;

    SetPacketWorkCursor(BuildSpriteQuadPackets(otAddress, ReservePacketWorkBytes(0),
                         sourceAddress, animationByte, frameByte, (s32) x,
                         (s32) y, control6, control6, control7, (s32) control8,
                         -1, -1, -1, -1));
}

/*
 * Sprite-submission wrapper: masks arg3 to a byte, records its caller's
 * return address into D_8006C84C (CAPTURE_RA debug hook), and forwards
 * all arguments to func_800165D8 with arg7 passed twice and the tail
 * padded with -1. See notes/research/caller-capture-debug-hook.md.
 */
void func_80016054(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s16 arg5,
                   s16 arg6, s32 arg7, s32 arg8, u16 arg9) {
    arg3 &= 0xFF;
    CAPTURE_RA(&D_8006C84C);
    BuildSpriteQuadPackets(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7,
                  arg7, 0, arg9, -1, -1, -1, -1);
}

void func_800160C8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7, u16 arg8, s16 arg9, s16 arg10, s16 arg11, s16 arg12) {
    s32 temp_a2;
    s32 temp_s3;

    temp_a2 = arg2 & 0xFF;
    temp_s3 = arg3 & 0xFF;
    SetPacketWorkCursor(BuildSpriteQuadPackets(arg0, ReservePacketWorkBytes(0), arg1, temp_a2, temp_s3, (s32)arg4, (s32)arg5, arg6, arg6, arg7, (s32)arg8, (s32)arg9, (s32)arg10, (s32)arg11, (s32)arg12));
}

void func_800161AC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7, u16 arg8) {
    s32 var_s8;
    s32 var_s0;

    var_s8 = arg2 & 0xFF;
    var_s0 = arg3 & 0xFF;
    SetPacketWorkCursor(BuildSpriteQuadPackets(arg0, ReservePacketWorkBytes(0), arg1, var_s8, var_s0, (s32)arg4, (s32)arg5, arg6, arg7, 0, (s32)arg8, -1, -1, -1, -1));
}

typedef struct {
    u16 field_00;
    s16 field_02;
    s16 field_04;
    s16 field_06;
    s16 field_08;
} SpriteRectHeader;

typedef struct {
    u16 field_00;
    u16 field_02;
    u16 field_04;
    u16 field_06;
    u8 field_08;
    u8 pad_09;
    u16 pad_0A;
} SpriteRectEntry;

/*
 * ASSEMBLY EXCEPTION (project-owner approved): only the compiler-coupled
 * entry/guard region is expressed with extended asm. The complete rendering
 * loop remains active C. The fixed-register outputs hand the target ABI state
 * directly to that C loop without duplicating or disabling it.
 */
void BuildSpriteRectPackets(u_long *arg0, SPRT **arg1, DR_MODE **arg2, SpriteRectSourceView *arg3,
                   u8 arg4, u8 arg5, s16 arg6, s16 arg7) {
    RECT rect;
    SpriteRectEntry *ent;
    SpriteRectVertex *uv0;
    SpriteRectVertex *uv1;
    s16 u;
    s16 w;
    s16 h;
    s32 v;
    u8 tp;
    s32 clutX;
    u32 clutY;
    s32 clutAdd;
    register u_long *ot __asm__("$17");
    register SPRT **sprtpp __asm__("$9");
    register DR_MODE **modepp __asm__("$16");
    register DR_MODE **modeIn __asm__("$6");
    register SpriteRectSourceView *src __asm__("$11");
    register u32 arg4Reg __asm__("$2");
    register s32 i __asm__("$14");
    register s16 xArg __asm__("$22");
    register s16 yArg __asm__("$21");
    register s16 hx __asm__("$20");
    register s16 hy __asm__("$19");

    modeIn = arg2;
    __asm__(
        "addu %0,$4,$zero\n\t"
        "addu %1,$5,$zero"
        : "=r"(ot), "=r"(sprtpp));
    __asm__(
        "lbu %0,56($sp)\n\t"
        "addu %1,$7,$zero"
        : "=r"(arg4Reg), "=r"(src)
        :
        : "$16");
    __asm__ volatile(
        ".set noreorder\n\t"
        ".set nomacro\n\t"
        "lw $3,40($11)\n\t"
        "lw $4,44($11)\n\t"
        "lh $22,64($sp)\n\t"
        "lh $21,68($sp)\n\t"
        "sll $2,$2,2\n\t"
        "addu $3,$3,$2\n\t"
        "lhu $5,2($3)\n\t"
        "lbu $3,60($sp)\n\t"
        "addu $4,$4,$5\n\t"
        "sll $2,$3,2\n\t"
        "addu $2,$2,$3\n\t"
        "sll $2,$2,1\n\t"
        "addu $7,$4,$2\n\t"
        "lhu $3,0($7)\n\t"
        "ori $2,$zero,0xfffd\n\t"
        "sltu $2,$2,$3\n\t"
        "bnez $2,.Lfunc_80016280_hybrid_epilogue\n\t"
        "addu $16,$6,$zero\n\t"
        "addu $2,$3,$zero\n\t"
        "sll $2,$2,2\n\t"
        "lw $3,32($11)\n\t"
        "lw $5,36($11)\n\t"
        "lh $20,2($7)\n\t"
        "lh $19,4($7)\n\t"
        "addu $3,$3,$2\n\t"
        "lhu $6,2($3)\n\t"
        "lh $4,0($3)\n\t"
        "addu $8,$5,$6\n\t"
        "sll $2,$4,1\n\t"
        "addu $2,$2,$4\n\t"
        "sll $2,$2,2\n\t"
        "addiu $2,$2,-12\n\t"
        "addiu $14,$4,-1\n\t"
        /* Encoding the branch directly prevents maspsx from inserting a
         * spurious nop between this branch and its target delay slot. */
        ".word 0x05c0009f\n\t"
        "addu $8,$8,$2\n\t"
        ".set macro"
        : "+r"(arg4Reg)
        : "r"(modeIn), "r"(src)
        : "$3", "$4", "$5", "$7", "$8", "$14", "$16",
          "$19", "$20", "$21", "$22", "memory");
    __asm__ volatile(""
        : "=r"(modepp), "=r"(ent), "=r"(i),
          "=r"(xArg), "=r"(yArg), "=r"(hx), "=r"(hy));

    do {
        uv0 = &src->field_1C[ent->field_00];
        u = (uv0->field_00 + src->field_0C) & 0x3F;
        w = uv0->field_04;
        h = uv0->field_06;
        uv1 = &src->field_1C[ent->field_02];
        v = (uv0->field_02 + src->field_0E) & 0xFF;
        tp = ent->field_08 >> 7;
        if (tp) {
            u *= 2;
            w *= 2;
        } else {
            u *= 4;
            w *= 4;
        }
        setSprt(*sprtpp);
        *(u_long *)&(*sprtpp)->r0 = 0x64808080;
        setSemiTrans(*sprtpp, (src->field_02 & 0x20) || ((ent->field_08 >> 4) & 1));
        setShadeTex(*sprtpp, 0);
        (*sprtpp)->x0 = ent->field_04 + (xArg + hx);
        (*sprtpp)->y0 = ent->field_06 + (yArg + hy);
        (*sprtpp)->w = w;
        (*sprtpp)->h = h;
        (*sprtpp)->u0 = u;
        (*sprtpp)->v0 = v;
        clutY = uv1->field_02;
        clutAdd = src->field_12;
        clutY += clutAdd;
        clutX = (s16)uv1->field_00;
        clutAdd = src->field_10;
        clutX += clutAdd;
        (*sprtpp)->clut = getClut(clutX, clutY);
        addPrim(ot, *sprtpp);
        (*sprtpp)++;
        setDrawTPage(*modepp, 1, 1,
                     ((tp & 0x3) << 7)
                     | ((((s16)uv0->field_02 + (s16)src->field_0E) & 0x100) >> 4)
                     | ((((s16)uv0->field_00 + (s16)src->field_0C) & 0x3C0) >> 6)
                     | ((((s16)uv0->field_02 + (s16)src->field_0E) & 0x200) << 2));
        addPrim(ot, *modepp);
        (*modepp)++;
        ent--;
        i--;
    } while (i >= 0);

    __asm__ volatile(".Lfunc_80016280_hybrid_epilogue:");
}

/* func_800165D8 — larger direct-primitive renderer (sprite-renderer family,
 * notes/file-groupings.md). Called by func_80015E78, func_80015F80,
 * func_80016054, func_800160C8, and func_800161AC.
 *
 * Uses baseline compiler settings. The former -mno-split-addresses
 * exception was withdrawn after declaration/assembler ownership was fixed
 * (notes/adr-0001-symbol-addressing-at-the-assembler-boundary.md).
 *
 * Web-shape choices that are load-bearing (do not "simplify"):
 * - flip2 = flip; flip = 0;  — the dead redefinition blocks CSE in-block
 *   copy forwarding and gcse copy-prop, so the copy survives to allocation
 *   (the target's `move a0,t6`); flow deletes the dead store. Same idiom as
 *   the SPRT renderer's work-variable kill.
 * - sx is ONE variable reused for both texture sums (X then Y). Its single
 *   local web keeps a1 busy through both halves, which steers u/u2/v into
 *   the target's registers via set_preference.
 * - u2/v2 are fresh variables for the post-getTPage masks; the bodies read
 *   them, so u/v die at the masks like the target's webs do.
 * - grp is an integer sum (index first) because pointer_int_sum would
 *   canonicalize the pointer operand first; ent is built in place.
 * See notes/research/func_800165D8-code-region-fold-and-allocation.md.
 */

typedef struct {
    u16 field_00;
    s16 field_02;
    s16 field_04;
    s16 field_06;
    s16 field_08;
} SpriteQuadHeader;

typedef struct {
    u16 field_00;
    u16 field_02;
    s16 field_04;
    s16 field_06;
    u8 field_08;
    u8 pad_09;
    u16 pad_0A;
} SpriteQuadEntry;

POLY_FT4 *BuildSpriteQuadPackets(u_long *arg0, POLY_FT4 *arg1, SpriteQuadSourceView *arg2, u8 arg3,
                        u8 arg4, s16 arg5, s16 arg6, s32 arg7, s32 arg8, s32 arg9,
                        u16 arg10, s16 arg11, s16 arg12, s16 arg13, s16 arg14) {
    POLY_FT4 *p;
    SpriteQuadHeader *hdr;
    SpriteRenderGroup *grp;
    SpriteQuadEntry *ent;
    SpriteQuadVertex *uv0;
    SpriteQuadVertex *uv1;
    s32 clutX;
    s32 clutY;
    s16 tpageX;
    s16 tpageY;
    s16 u;
    s16 v;
    s16 u2;
    s16 v2;
    s16 w;
    s16 h;
    s32 sx;
    s32 sy;
    s32 tp;
    s32 flip;
    s32 flip2;
    s32 xBase;
    s32 xFar;
    s32 yBase;
    s32 yFar;
    s16 x0;
    s16 x1;
    s16 y0;
    s16 y1;
    s16 i;
    s16 count;

    p = arg1;
    hdr = (SpriteQuadHeader *)(arg2->field_2C + arg2->field_28[arg3].field_02) + arg4;
    if (hdr->field_00 >= 0xFFFE) {
        return p;
    }

    grp = (SpriteRenderGroup *)(hdr->field_00 * 4 + (u32)arg2->field_20);
    count = grp->field_00;
    ent = (SpriteQuadEntry *)(arg2->field_24 + grp->field_02);
    ent += count - 1;
    i = count - 1;
    while (i >= 0) {
        uv0 = &arg2->field_1C[ent->field_00];
        uv1 = &arg2->field_1C[ent->field_02];
        setPolyFT4(p);
        setSemiTrans(p, (arg2->field_02 & 0x20) || ((ent->field_08 >> 4) & 1));
        setShadeTex(p, 0);
        setRGB0(p, 0x80, 0x80, 0x80);

        if (arg13 == -1) {
            clutX = uv1->field_00 + arg2->field_10;
            clutY = uv1->field_02 + arg2->field_12;
        } else {
            clutX = arg13;
            clutY = arg14;
        }
        p->clut = getClut(clutX, clutY);

        tp = ent->field_08 >> 7;
        flip = ent->field_08 & 3;
        if (arg11 == -1) {
            sx = uv0->field_00 + arg2->field_0C;
            tpageX = sx & ~0x3F;
            u = sx & 0x3F;
            sx = uv0->field_02 + arg2->field_0E;
            tpageY = sx & ~0xFF;
            v = sx & 0xFF;
        } else {
            u = arg11;
            v = arg12;
            tpageX = arg11;
            tpageY = arg12;
        }
        p->tpage = getTPage(tp, 0, tpageX, tpageY);

        u2 = u & 0x3F;
        v2 = v & 0xFF;
        w = uv0->field_04;
        h = uv0->field_06;
        if (tp == 0) {
            u2 *= 4;
            w *= 4;
        } else {
            u2 *= 2;
            w *= 2;
        }

        flip2 = flip;
        flip = 0;
        if (flip2 == 0) {
            p->u0 = u2;
            p->v0 = v2;
            p->u1 = u2 + (w - 1);
            p->v1 = v2;
            p->u2 = u2;
            p->v2 = v2 + (h - 1);
            p->u3 = u2 + (w - 1);
            p->v3 = v2 + (h - 1);
        } else if (flip2 == 1) {
            p->u0 = u2 + (w - 1);
            p->v0 = v2;
            p->u1 = u2;
            p->v1 = v2;
            p->u2 = u2 + (w - 1);
            p->v2 = v2 + (h - 1);
            p->u3 = u2;
            p->v3 = v2 + (h - 1);
        } else if (flip2 == 2) {
            p->u0 = u2;
            p->v0 = v2 + (h - 1);
            p->u1 = u2 + (w - 1);
            p->v1 = v2 + (h - 1);
            p->u2 = u2;
            p->v2 = v2;
            p->u3 = u2 + (w - 1);
            p->v3 = v2;
        } else {
            p->u0 = u2 + (w - 1);
            p->v0 = v2 + (h - 1);
            p->u1 = u2;
            p->v1 = v2 + (h - 1);
            p->u2 = u2 + (w - 1);
            p->v2 = v2;
            p->u3 = u2;
            p->v3 = v2;
        }

        if (arg9 == 0) {
            xBase = hdr->field_02 + ent->field_04;
            xFar = xBase + w;
            yBase = hdr->field_04 + ent->field_06;
            yFar = yBase + h;
            if (arg7 + arg8 != 0x2000) {
                x0 = arg5 + ((xBase * arg7) / 4096);
                x1 = arg5 + ((xFar * arg7) / 4096);
                y0 = arg6 + ((yBase * arg8) / 4096);
                y1 = arg6 + ((yFar * arg8) / 4096);
            } else {
                x0 = arg5 + xBase;
                x1 = arg5 + xFar;
                y0 = arg6 + yBase;
                y1 = arg6 + yFar;
            }
            if (arg10 & 0x18) {
                p->x0 = x1;
                p->y0 = y1;
                p->x1 = x0;
                p->y1 = y1;
                p->x2 = x1;
                p->y2 = y0;
                p->x3 = x0;
                p->y3 = y0;
            } else if (arg10 & 8) {
                p->x0 = x1;
                p->y0 = y0;
                p->x1 = x0;
                p->y1 = y0;
                p->x2 = x1;
                p->y2 = y1;
                p->x3 = x0;
                p->y3 = y1;
            } else if (arg10 & 0x10) {
                p->x0 = x0;
                p->y0 = y1;
                p->x1 = x1;
                p->y1 = y1;
                p->x2 = x0;
                p->y2 = y0;
                p->x3 = x1;
                p->y3 = y0;
            } else {
                p->x0 = x0;
                p->y0 = y0;
                p->x1 = x1;
                p->y1 = y0;
                p->x2 = x0;
                p->y2 = y1;
                p->x3 = x1;
                p->y3 = y1;
            }
        }

        if (arg10 & 0x40) {
            p->tag = 0x09000000 | getaddr(arg0);
            *arg0 = (u_long)p & 0xFFFFFF;
            gActiveRenderContext->field_118 += 0x28;
        } else {
            addPrim(arg0, p);
        }
        p++;
        ent--;
        i--;
    }
    return p;
}

u16 FindSpriteTextureRecord(s32 arg0, s32 arg1, u16 arg2);                    /* extern */
s32 UploadSpriteImageEntry(u8 *arg0, u8 *arg1, u16 arg2, s16 arg3, s16 arg4);

s32 UploadSpriteEntryAndGetPacketBytes(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 found;
    s32 size;

    found = FindSpriteTextureRecord(arg0, (s32 *)arg0[6], arg1);
    size = UploadSpriteImageEntry((s32 *)arg0[6], arg2, found, arg3, arg4);
    return size / 4 * 4 + 0x20;
}

/* Research, evidence, and falsified hypotheses:
 * notes/research/func_80016C08-tu-owned-globals-and-gp-relative-addressing.md
 */

/* Sprite entry loop driver: walks the entry list of one sprite frame and
 * emits a POLY_FT4 per entry, accumulating texture upload sizes through
 * func_80016B7C.
 *
 * Baseline flags: the former -mno-split-addresses exception was withdrawn
 * after declaration/assembler ownership was fixed (ADR-0001).
 * The redundant found=nclut assignment preserves the compiler's spill-slot
 * layout. Keep it until that source-sensitive construction is understood.
 */

u16 gSpriteEntryTextureId;

s32 UploadSpriteEntryAndGetPacketBytes(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4); /* extern */

/* one drawn piece of a frame (12 bytes) */
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u8  unk8;
    /* 0x09 */ u8  unk9;
    /* 0x0A */ u16 unkA;
} SpriteEntry;

/* one animation frame header (10 bytes) */
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u16 unk8;
} SpriteFrame;

POLY_FT4* BuildSpriteEntryPackets(s32 *ot, POLY_FT4 *poly, SpriteEntrySourceView *src,
                        s16 ox, s16 oy, u16 flags, s32 total, s32 texBase,
                        s16 subst, s16 substFrom, s16 substTo) {
    s16 clutList[12];
    SpriteFrame *frame;
    SpriteEntry *ent;
    SpriteEntryTexture *cel;
    SpriteEntryTexture *tex;
    s32 count;
    s32 nclut;
    s32 found;
    s32 size;
    s32 key;
    s32 tp;
    s32 rot;
    s32 i;
    s32 j;
    s32 fill;
    s32 last;
    s16 tx;
    s16 ty;
    s16 u;
    s16 v;
    s16 w;
    s16 h;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s32 xbase;
    s32 ybase;

    frame = (SpriteFrame *) (src->unk2C + src->unk28[src->unk4].unk2) + src->unk5;
    if (frame->unk0 < 0xFFFE) {
        nclut = 0;
        for (fill = 0; fill < 10; fill++) {
            clutList[fill] = -1;
        }
        ent = (SpriteEntry *) (src->unk24 + src->unk20[frame->unk0].unk2);
        count = src->unk20[frame->unk0].unk0;
        ent += count - 1;
        last = count - 1;
        i = last;
        for (; i >= 0; i--, ent--) {
            size = 0;
            /* Tracked debt: this redundant store only shifts GCSE
             * pseudo-register numbering so reload assigns the target's spill
             * slot layout; see the file header comment. Do not remove
             * without re-checking the 72/76/80/84/88 slot map. */
            found = nclut;
            found = 0;
            cel = &src->unk1C[ent->unk0];
            tex = &((SpriteEntryTexture *) texBase)[ent->unk9 - 0x80];
            key = ent->unk9 - 0x80;
            for (j = 0; j < 10; j++) {
                if (clutList[j] == key) {
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                clutList[nclut++] = ent->unk9 - 0x80;
                size = UploadSpriteEntryAndGetPacketBytes((s32 *) src, ent->unk0, total, tex->unk4, tex->unk6);
                gSpriteEntryTextureId = ent->unk2;
                if (subst != -1) {
                    if (ent->unk9 == 0x80) {
                        gSpriteEntryTextureId = subst;
                    }
                    if (ent->unk2 == substFrom) {
                        gSpriteEntryTextureId = substTo;
                    }
                }
                size += UploadSpriteEntryAndGetPacketBytes((s32 *) src, (s16) gSpriteEntryTextureId, total + size,
                                      tex->unk0, tex->unk2);
            }
            setPolyFT4(poly);
            setSemiTrans(poly, (ent->unk8 >> 4) & 1);
            setRGB0(poly, 0x80, 0x80, 0x80);
            setShadeTex(poly, 0);
            setClut(poly, tex->unk0, tex->unk2);
            tx = tex->unk4;
            ty = tex->unk6;
            u = tx & 0x3F;
            tp = ent->unk8 >> 7;
            rot = ent->unk8 & 3;
            setTPage(poly, tp, 0, tx, ty);
            w = cel->unk4;
            if (tp == 0) {
                u *= 4;
                w *= 4;
            } else {
                u *= 2;
                w *= 2;
            }
            v = ty & 0xFF;
            h = cel->unk6;
            if (rot == 0) {
                setUV4(poly, u, v, u + (w - 1), v, u, v + (h - 1), u + (w - 1), v + (h - 1));
            } else if (rot == 1) {
                setUV4(poly, u + (w - 1), v, u, v, u + (w - 1), v + (h - 1), u, v + (h - 1));
            } else if (rot == 2) {
                setUV4(poly, u, v + (h - 1), u + (w - 1), v + (h - 1), u, v, u + (w - 1), v);
            } else {
                setUV4(poly, u + (w - 1), v + (h - 1), u, v + (h - 1), u + (w - 1), v, u, v);
            }
            xbase = ox + frame->unk2;
            x0 = xbase + ent->unk4;
            x1 = x0 + w;
            ybase = oy + frame->unk4;
            y0 = ybase + ent->unk6;
            y1 = y0 + h;
            if (flags & 0x18) {
                setXY4(poly, x1, y1, x0, y1, x1, y0, x0, y0);
            } else if (flags & 8) {
                setXY4(poly, x1, y0, x0, y0, x1, y1, x0, y1);
            } else if (flags & 0x10) {
                setXY4(poly, x0, y1, x1, y1, x0, y0, x1, y0);
            } else {
                setXY4(poly, x0, y0, x1, y0, x0, y1, x1, y1);
            }
            poly->tag = (*ot & 0xFFFFFF) | 0x09000000;
            *ot = (s32) poly & 0xFFFFFF;
            total += size;
            poly++;
            gActiveRenderContext->field_118 += 0x28;
        }
    }
    return poly;
}

void DecodeAndUploadSpriteImages(u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5);

void UploadSpriteImages(u8 *arg0) {
    DecodeAndUploadSpriteImages(arg0, 0, 0, 0, 0, 0);
}

void UploadSpriteImagesWithMask(s32 arg0) {
    DecodeAndUploadSpriteImages(((void *)arg0), 0, 0, 0, 0, 1);
}

void UploadSpriteImagesAtOffsets(u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    DecodeAndUploadSpriteImages(arg0, arg1, arg2, arg3, arg4, 0);
}

void UploadSpriteImagesKeepingWideType2Positions(u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    DecodeAndUploadSpriteImages(arg0, arg1, arg2, arg3, (s16) (s32) arg4, 2);
}

/* Upload one sprite frame: resolve the frame-entry index of the requested
 * animation/frame pair, find the matching texture record, and let the tile
 * loader decode that record into the D_8005EA28 image buffer.
 */
void func_80017284(SpriteSourceData *source, u8 *image, s32 animation, s32 frame,
                   s16 x, s16 y) {
    u16 entry;
    u16 record;

    entry = GetSpriteFrameEntryIndex1(source, animation & 0xFF, frame & 0xFF);
    record = FindSpriteTextureRecord((s32)source, (s32)image, entry);
    UploadSpriteImageEntry(image, (u8 *)&D_8005EA28, record, x, y);
}

void DecodeAndUploadSpriteImages(u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5) {
    RECT rect;
    u16 count;
    u32 entry_data;
    u32 header;
    u32 size;
    u32 bytes;
    u8 *src;
    u8 *dst;
    u32 entry_idx;
    s8 count_rle;
    u8 repeat;
    u8 repeat_val = 0;
    s32 row_width;
    u16 height;
    s32 row;
    s32 next_row;
    u16 channel;
    u16 pixel;
    u32 flags2;

    if (*arg0 != 0xD) {
        return;
    }

    count = *(u16 *)(arg0 + 2);
    arg0 += 4;

    for (entry_idx = 0; entry_idx < count; entry_idx = (entry_idx + 1) & 0xFFFF) {
        header = *(u32 *)arg0;
        size = header & 0xFFFFFF;
        entry_data = header >> 24;
        src = arg0 + 12;
        rect.x = *(u16 *)(arg0 + 4);
        rect.y = *(u16 *)(arg0 + 6);
        rect.w = *(u16 *)(arg0 + 8);
        rect.h = *(u16 *)(arg0 + 10);

        if ((entry_data & 3) != 2) {
            rect.x += arg1;
            rect.y += arg2;
        } else if (((s16)rect.w < 0x100) || (arg5 != (entry_data & 3))) {
            rect.x += arg3;
            rect.y += arg4;
        }

        count_rle = 0;
        repeat = 0;
        row_width = (rect.w * 2) & 0xFFFF;
        height = (u16)rect.h;

        if (!(entry_data & 0xC)) {
            rect.h = 1;
            row = 0;
            if (height != 0) {
                do {
                    dst = D_8005EE28;
                    next_row = row + 1;
                    row = 0;
                    while (row < row_width) {
                        if (count_rle <= 0) {
                            count_rle = *(s8 *)src++;
                            if (count_rle < 0) {
                                repeat = 0;
                                count_rle = (s8)(-count_rle);
                            } else {
                                repeat = 1;
                                repeat_val = *src++;
                            }
                        }
                        if (!repeat) {
                            *dst = *src++;
                            dst++;
                        } else {
                            *dst = repeat_val;
                            dst++;
                        }
                        row++;
                        count_rle--;
                    }

                    flags2 = entry_data & 2;
                    if (flags2 && (arg5 == 1)) {
                        pixel = 1;
                        while (pixel < rect.w) {
                            ((u16 *)D_8005EE28)[pixel] |= 0x8000;
                            pixel++;
                        }
                    }

                    LoadImage(&rect, (u_long *)D_8005EE28);
                    row = next_row;
                    rect.y++;
                } while (row < height);
            }
        } else if (entry_data & 8) {
            rect.w = 1;
            row = 0;
            if (row_width != 0) {
                do {
                    channel = 0;
                    next_row = row + 2;
                    do {
                        dst = D_8005EE28 + (channel != 0);
                        if (height != 0) {
                            row = height;
                            do {
                                if (count_rle <= 0) {
                                    count_rle = *(s8 *)src++;
                                    if (count_rle < 0) {
                                        repeat = 0;
                                        count_rle = (s8)(-count_rle);
                                    } else {
                                        repeat = 1;
                                        repeat_val = *src++;
                                    }
                                }
                                if (!repeat) {
                                    *dst = *src++;
                                } else {
                                    *dst = repeat_val;
                                }
                                dst += 2;
                                row--;
                                count_rle--;
                            } while (row != 0);
                        }
                        channel++;
                    } while (channel < 2);

                    flags2 = entry_data & 2;
                    if (flags2 && (arg5 == 1)) {
                        pixel = 1;
                        while (pixel < rect.w) {
                            ((u16 *)D_8005EE28)[pixel] |= 0x8000;
                            pixel++;
                        }
                    }

                    LoadImage(&rect, (u_long *)D_8005EE28);
                    row = next_row;
                    rect.x++;
                } while (row < row_width);
            }
        } else {
            rect.h = 1;
            row = 0;
            if (height != 0) {
                do {
                    dst = D_8005EE28;
                    next_row = row + 1;
                    for (row = 0; row < row_width; row++) {
                        *dst = *src++;
                        dst++;
                    }

                    flags2 = entry_data & 2;
                    if (flags2 && (arg5 == 1)) {
                        pixel = 1;
                        while (pixel < rect.w) {
                            ((u16 *)D_8005EE28)[pixel] |= 0x8000;
                            pixel++;
                        }
                    }

                    LoadImage(&rect, (u_long *)D_8005EE28);
                    row = next_row;
                    rect.y++;
                } while (row < height);
            }
        }

        do {
        } while (DrawSync(1) != 0);

        bytes = ((size + 3) >> 2) << 2;
        arg0 += bytes + 12;
    }
}

s32 UploadSpriteImageEntry(u8 *arg0, u8 *arg1, u16 arg2, s16 arg3, s16 arg4) {
    u8 header;
    u16 count;
    u32 tile_idx;
    u32 entry_data;
    u32 size;
    u8 mode;
    u8 *src;
    u32 row_width;
    u8 *dst;
    s8 count_rle;
    u8 repeat;
    u8 repeat_val;
    s32 row;
    s32 next_row;
    s32 hh;
    u16 height;
    RECT rect;

    header = *arg0;
    if (header != 0xD) {
        return 0;
    }

    count = *(u16 *)(arg0 + 2);
    arg0 += 4;

    for (tile_idx = 0; tile_idx < count; tile_idx = (tile_idx + 1) & 0xFFFF) {
        entry_data = *(u32 *)arg0;
        size = entry_data & 0xFFFFFF;
        if (tile_idx != arg2) {
            arg0 = (u8 *)arg0 + ((((size + 3) >> 2) << 2) + 12);
            continue;
        }
        mode = entry_data >> 24;
        if (arg3 == -1) {
            rect.x = *(u16 *)(arg0 + 4);
            row = *(u16 *)(arg0 + 6);
            rect.y = row;
        } else {
            rect.x = arg3;
            rect.y = arg4;
        }
        src = (u8 *)arg0 + 12;
        rect.w = *(u16 *)(arg0 + 8);
        rect.h = *(u16 *)(arg0 + 10);
        if (!(mode & 0xC)) {
            count_rle = 0;
            repeat = 0;
            repeat_val = 0;
            dst = arg1;
            row = 0;
            row_width = (rect.w * 2) & 0xFFFE;
            height = (u16)rect.h;
            if (height != 0) {
                do {
                    next_row = row + 1;
                    if (row_width != 0) {
                        row = row_width;
                        do {
                            if (count_rle <= 0) {
                                count_rle = *(s8 *)src++;
                                if (count_rle < 0) {
                                    repeat = 0;
                                    count_rle = (s8)(-count_rle);
                                } else {
                                    repeat = 1;
                                    repeat_val = *src++;
                                }
                            }
                            if (!repeat) {
                                *dst++ = *src++;
                            } else {
                                *dst++ = repeat_val;
                            }
                            row--;
                            count_rle--;
                        } while (row != 0);
                    }
                    row = next_row;
                } while (row < height);
            }
            LoadImage(&rect, (u_long *)arg1);
        } else {
            LoadImage(&rect, (u_long *)src);
        }
        break;
    }

    hh = rect.h * 2;
    return rect.w * hh;
}
