#ifndef VRAM_CLEAR_H
#define VRAM_CLEAR_H

#include "common.h"

/* Clear a VRAM rectangle to RGB and wait for GPU completion (DrawSync(0)).
 * x/width use 16-bit VRAM-word columns, y/height use rows, not necessarily
 * display pixels for packed texture formats. Rectangle arguments narrow to
 * signed 16 bits; RGB is unsigned 8-bit. The caller supplies a valid rectangle:
 * no clipping, range validation or VBlank synchronization is added here.
 * Original identity: exe:0x80012A68. */
void ClearVramRectAndWait(s32 x, s32 y, s32 width, s32 height,
                         u8 red, u8 green, u8 blue);

#endif
