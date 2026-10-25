/*
 * Opaque fixed-palette panels: a two-pixel beveled border around a vertical
 * gradient. All drawing goes through screen_primitives and its packet cursor;
 * this module has no persistent state. See include/beveled_panel.h.
 */

#include "common.h"
#include "beveled_panel.h"
#include "screen_primitives.h"

void DrawBeveledPanel(u32 *ot, s16 x, s16 y, s16 width, s16 height) {
    s32 widthMinusOne;
    s32 innerRight;
    s16 innerLineRight;
    s16 innerBottom;
    s16 heightMinusOne;
    s16 innerLeft;
    s16 innerTop;
    s16 bottom;
    s32 xMinusOne;
    s32 right;

    widthMinusOne = (s16)(width - 1);
    xMinusOne = x - 1;
    innerRight = (s16)(widthMinusOne + xMinusOne);
    heightMinusOne = height - 1;
    DrawScreenPoint(ot, innerRight, y, 0xF8DBAF, 0);
    innerTop = y + 1;
    DrawScreenPoint(ot, innerRight, innerTop, 0xDEBB84, 0);
    right = x + widthMinusOne;
    DrawScreenPoint(ot, (s16)right, innerTop, 0xC89F60, 0);
    innerLeft = x + 1;
    innerBottom = heightMinusOne - 1 + y;
    DrawScreenPoint(ot, innerLeft, innerBottom, 0xE2C08B, 0);
    bottom = y + heightMinusOne;
    DrawScreenPoint(ot, innerLeft, bottom, 0xC89F60, 0);
    DrawScreenPoint(ot, x, innerBottom, 0xF8DBAF, 0);
    innerLineRight = (s16)(widthMinusOne - 2 + x);
    {
        s32 shadowColor;

        DrawScreenLine(ot, innerLeft, innerTop, innerLineRight, innerTop, 0xFDECD2, 0);
        DrawScreenLine(ot, innerLeft, innerTop, innerLeft, innerBottom, 0xFDECD2, 0);
        /* Keep this assignment at first use: moving it to the declaration
         * changes constant materialization against the highlight color. */
        shadowColor = 0xBA8B47;
        DrawScreenLine(ot, innerRight, innerTop, innerRight, innerBottom, shadowColor, 0);
        DrawScreenLine(ot, innerLeft, innerBottom, innerLineRight, innerBottom, shadowColor, 0);
        DrawScreenLine(ot, innerLeft, y, (s16)innerRight, y, 0xFDECD2, 0);
        DrawScreenLine(ot, x, innerTop, x, innerBottom, 0xFDECD2, 0);
        DrawScreenLine(ot, (s16)right, innerTop, (s16)right, innerBottom, shadowColor, 0);
        DrawScreenLine(ot, innerLeft, bottom, (s16)innerRight, bottom, shadowColor, 0);
    }
    DrawScreenGradientRect((s32 *)ot, (s16)(x + 2), (s16)(y + 2),
                           widthMinusOne - 3, heightMinusOne - 3,
                           0xF4DFBD, 0xF4DFBD, 0xF9CF8F, 0xF9CF8F, 0);
}
