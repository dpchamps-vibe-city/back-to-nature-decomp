/* Capture, interpolate and upload two 16-pixel RGB555 rows.
 * The record and source rows belong to the caller; only the unreferenced
 * sequence counter below is persistent module state. See color_row_blend.h
 * for timing, buffer lifetime and packet-pool requirements, and
 * notes/file-groupings.md for membership/boundary evidence. */
#include "color_row_blend.h"
#include "render_packet_pool.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 GetColorRowYOffset(void);
void InterpolateColorRow(u16 *out, u16 *first, u16 *second, s32 progress, s32 span);
void func_8001F278(s32 progress, s32 span, s32 *first, s32 *second, s32 *out);

void InitColorRowBlend(ColorRowBlend *blend, u_long *rows, s16 span,
                      s32 src0X, s32 src0Y, s32 src1X, s32 src1Y,
                      s32 dstX, s32 dstY) {
    RECT rect;
    s32 yOffset;

    blend->sourceRowsAddress = (u_long)rows;
    blend->direction = -2;
    blend->span = span;
    blend->reserved6 = 0;
    blend->progress = 0;
    yOffset = GetColorRowYOffset();
    DrawSync(0);
    rect.x = src0X;
    rect.w = 0x10;
    rect.h = 1;
    rect.y = src0Y + yOffset;
    StoreImage(&rect, rows);
    DrawSync(0);
    rect.x = src1X;
    rect.y = src1Y + yOffset;
    rect.w = 0x10;
    rect.h = 1;
    StoreImage(&rect, rows + 8);
    DrawSync(0);
    blend->dstX = (s16)dstX;
    blend->dstY = (s16)(dstY + yOffset);
    blend->width = 0x10;
    blend->height = 1;
}

void InterpolateColorRow(u16 *out, u16 *first, u16 *second, s32 progress, s32 span) {
    s32 firstChannels[3];
    s32 secondChannels[3];
    s32 resultChannels[3];
    /* Keep these array-address initializers before the cursor copies: their
     * early lifetimes preserve the original call/save-register arrangement. */
    s32 *secondVector = secondChannels;
    s32 *resultVector = resultChannels;
    u16 *outCursor;
    u16 *firstCursor;
    u16 *secondCursor;
    s32 remaining;
    u32 firstColor;
    u32 firstWord;
    u32 secondColor;
    u32 secondWord;
    s16 packed;
    u16 middleChannel;

    outCursor = out;
    secondCursor = second;
    firstCursor = first;
    remaining = 0xF;
    do {
        firstColor = *firstCursor;
        firstWord = firstColor & 0xFFFF;
        firstChannels[2] = firstColor & 0x1F;
        firstChannels[1] = (firstWord >> 5) & 0x1F;
        firstChannels[0] = (firstWord >> 0xA) & 0x1F;
        secondColor = *secondCursor;
        secondWord = secondColor & 0xFFFF;
        secondChannels[2] = secondColor & 0x1F;
        secondChannels[1] = (secondWord >> 5) & 0x1F;
        secondChannels[0] = (secondWord >> 0xA) & 0x1F;
        func_8001F278(progress, span, firstChannels, secondVector, resultVector);
        packed = resultChannels[0] & 0x1F;
        middleChannel = resultChannels[1];
        packed <<= 5;
        packed |= middleChannel & 0x1F;
        packed <<= 5;
        packed |= resultChannels[2] & 0x1F;
        if (packed != 0) {
            packed |= ~0x7FFF;
        }
        *outCursor = packed;
        outCursor++;
        secondCursor++;
        remaining--;
        firstCursor++;
    } while (remaining >= 0);
}

void StepColorRowBlend(ColorRowBlend *blend, s32 step, s32 mode) {
    RECT rect;
    char *packet;
    char *rows;
    s16 span;
    u16 spanBits;

    if (blend->span > 0) {
        if (blend->progress < 0) {
            blend->progress = 0;
        }
        if (blend->direction < 0) {
            blend->direction = 0;
        }
        span = blend->span;
        spanBits = (u16)blend->span;
        if (span < step) {
            step = span;
        }
        if (mode == 1) {
            if (blend->direction != 0) {
                blend->progress = (s16)((u16)blend->progress - step);
                if (blend->progress < 0) {
                    blend->progress = 0;
                    blend->direction = 0;
                }
            } else {
                blend->progress = (s16)((u16)blend->progress + step);
                if (span < blend->progress) {
                    blend->progress = spanBits;
                    blend->direction = mode;
                }
            }
        } else {
            s32 sum = (u16)blend->progress + step;
            step = sum;
            blend->progress = (s16)sum;
            if ((s16)sum < span) {
            } else {
                blend->progress = sum - spanBits;
            }
        }
        rect.x = blend->dstX;
        rect.y = blend->dstY;
        rect.w = blend->width;
        rect.h = blend->height;
        packet = (char *)LinkAndReservePacket(0, 0x44);
        SetDrawLoad((DR_LOAD *)packet, &rect);
        rows = (char *)blend->sourceRowsAddress;
        InterpolateColorRow((u16 *)(packet + 0x10), (u16 *)rows, (u16 *)(rows + 0x20),
                            blend->progress, blend->span);
    }
}

void DrawColorRowBlend(ColorRowBlend *blend, s32 progress) {
    RECT rect;
    char *packet;
    char *rows;

    rect.x = blend->dstX;
    rect.y = blend->dstY;
    rect.w = blend->width;
    rect.h = blend->height;
    packet = (char *)LinkAndReservePacket(0, 0x44);
    SetDrawLoad((DR_LOAD *)packet, &rect);
    rows = (char *)blend->sourceRowsAddress;
    InterpolateColorRow((u16 *)(packet + 0x10), (u16 *)rows, (u16 *)(rows + 0x20),
                        progress, blend->span);
}

/* Sole tentative definition: NextSequenceNumber accesses this word via $gp. */
s32 gSequenceNumber;

s32 NextSequenceNumber(void) {
    return gSequenceNumber++;
}

/* Retained offset hook; all three captured/destination Y coordinates use it. */
s32 GetColorRowYOffset(void) {
    return 0;
}
