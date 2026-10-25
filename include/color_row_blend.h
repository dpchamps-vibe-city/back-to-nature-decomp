#ifndef COLOR_ROW_BLEND_H
#define COLOR_ROW_BLEND_H

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

/* Capture two 16x1 VRAM rows into 0x40 caller-owned bytes (16 u_long words).
 * Coordinates are VRAM pixels. Waits for GPU completion before/between/after
 * reads, initializes progress/direction and a 16x1 upload destination.
 * The row buffer must remain valid for subsequent step/draw calls. */
void InitColorRowBlend(ColorRowBlend *blend, u_long *rows, s16 span,
                      s32 src0X, s32 src0Y, s32 src1X, s32 src1Y,
                      s32 dstX, s32 dstY);

/* For span > 0, advance progress and queue one 0x44-byte row-upload packet in
 * the active render context. mode==1 bounces; otherwise subtract span on wrap.
 * step clamps above to span, not below to zero. Bounce direction changes only
 * after crossing an endpoint; halfword wrapping and integer truncation remain.
 * Does nothing for span <= 0. Requires a usable packet pool. */
void StepColorRowBlend(ColorRowBlend *blend, s32 step, s32 mode);

/* Queue the same upload without changing progress. Requires span != 0;
 * progress clamps only above to span. Channels interpolate as RGB555, rounding
 * by signed integer division; output bit 15 is set iff the packed color != 0. */
void DrawColorRowBlend(ColorRowBlend *blend, s32 progress);

#endif
