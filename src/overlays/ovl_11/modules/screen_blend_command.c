/*
 * screen_blend_command — reconstructed translation unit.
 *
 * Object-program command that enqueues a semitransparent 320x240 color
 * rectangle into one of two active ordering-table slots. It owns no
 * persistent state.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "screen_primitives.h"
#include "ovl11_screen_blend_command.h"

s32 Ovl11DrawScreenBlendCommand(s16 layer, s16 red, s32 green, s32 blue) {
    s32 otWord;

    otWord = 0;
    if (layer != 0) {
        otWord = 5;
    }
    DrawScreenFlatRect((s32 *)(gActiveRenderContext->field_D8 + otWord * 4), 0, 0, 0x140, 0xF0,
                  (blue & 0xFF) | ((green & 0xFFFF) << 8) |
                      ((red & 0xFF) << 0x10),
                  1);
    return 1;
}
