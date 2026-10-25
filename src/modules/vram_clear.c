/*
 * vram_clear — reconstructed translation unit.
 *
 * Synchronous VRAM rectangle clear. Shared by boot and framebuffer feedback;
 * rectangle coordinates and dimensions are in VRAM units.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "vram_clear.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

void ClearVramRectAndWait(s32 x, s32 y, s32 width, s32 height, u8 red, u8 green, u8 blue) {
    RECT rect;

    rect.x = (s16)x;
    rect.y = (s16)y;
    rect.w = (s16)width;
    rect.h = (s16)height;
    ClearImage(&rect, red, green, blue);
    DrawSync(0);
}
