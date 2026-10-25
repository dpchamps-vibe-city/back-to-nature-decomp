/* Four-row paired-value display. Numeric totals use the first pair; each
 * row draws two opposed modulo-10000 bars and a threshold-selected label.
 * The shared text scratch is external, not private state of this module.
 * Membership/boundary evidence: notes/file-groupings.md. */
#include "paired_value_panel.h"
#include "screen_primitives.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);
void ovl_11_func_800FE614(u16 *arg0, s32 arg1);
void Ovl11DrawPairedValueRow(s16 row, s16 y);
void Ovl11DrawPairedValueBars(s32 *pair, s16 y);
void func_80024A10(s32 arg0, s16 arg1, s16 arg2, s16 arg3);
void ovl_11_func_800F7094(s32 arg0, s16 arg1, s16 arg2, s32 arg3);

void Ovl11DrawPairedValuePanel(void) {
    s16 rowY;
    s32 row;
    s32 textOt;
    s32 clampedValue;
    s32 value;

    textOt = gActiveRenderContext->field_D8 + 0x54;
    ((struct Ovl11NumberTextHeader *) &gOvl11NumberTextScratch)->marker = 0xFFD;
    func_80017B3C(textOt, (s32) (D_80053636 + gTextBankOffsets[0]), 0xB2, 0x14);
    func_80017B3C(textOt, (s32) (D_80053636 + 0x28 + gTextBankOffsets[0]), 0x20, 0x40);
    value = ((struct Ovl11PairedValues *) D_8006C838)->pairs[0][0];
    if (value >= -9999999) {
        clampedValue = value;
        if (clampedValue > 9999999) {
            clampedValue = 9999999;
        }
    } else {
        clampedValue = -9999999;
    }
    gOvl11NumberTextScratch = 0x68;
    ovl_11_func_800FE614((u16 *) func_8001A970(clampedValue, &gOvl11NumberTextScratch + 2, 7), 0x1E86);
    func_80017B3C(textOt, (s32) &gOvl11NumberTextScratch, 0xAC, 0x40);
    value = ((struct Ovl11PairedValues *) D_8006C838)->pairs[0][1];
    if (value >= -9999999) {
        clampedValue = value;
        if (clampedValue > 9999999) {
            clampedValue = 9999999;
        }
    } else {
        clampedValue = -9999999;
    }
    gOvl11NumberTextScratch = 0x69;
    ovl_11_func_800FE614((u16 *) func_8001A970(clampedValue, &gOvl11NumberTextScratch + 2, 7), 0x1E86);
    func_80017B3C(textOt, (s32) &gOvl11NumberTextScratch, 0xAC, 0x4E);
    rowY = 0x68;
    for (row = 0; row < 4; row++) {
        Ovl11DrawPairedValueRow(row, rowY);
        rowY += 0x20;
    }
}

void Ovl11DrawPairedValueRow(s16 row, s16 y) {
    s16 iconFrame;
    s32 secondValue;
    s32 firstBand;
    s32 test;
    s32 secondBand;
    void *label;
    s32 textOt = gActiveRenderContext->field_D8 + 0x54;

    iconFrame = ((struct Ovl11PairedValues *) D_8006C838)->field_44BA;
    iconFrame += 4;
    iconFrame -= row;
    func_80024A10(textOt, 0x18, y, iconFrame % 4);
    Ovl11DrawPairedValueBars(((struct Ovl11PairedValues *) D_8006C838)->pairs[row], y + 2);
    secondValue = ((struct Ovl11PairedValues *) D_8006C838)->pairs[row][1];
    secondBand = 0;
    if (secondValue < 0x1388) {
        secondBand = 2;
        if (secondValue >= 0x7D0) {
            secondBand = 1;
        }
    }
    test = ((struct Ovl11PairedValues *) D_8006C838)->pairs[row][0] < 0x1388;
    firstBand = 0;
    if (test) {
        firstBand = 2;
        if (((struct Ovl11PairedValues *) D_8006C838)->pairs[row][0] >= 0x7D0) {
            firstBand = 1;
        }
    }
    test = ((struct Ovl11PairedValues *) D_8006C838)->pairs[row][0];
    if (test == 0 &&
        ((struct Ovl11PairedValues *) D_8006C838)->pairs[row][1] == 0) {
        label = D_80053770 + gTextBankOffsets[0];
    } else {
        label = (u8 *) D_8005175C + (gTextBankOffsets[0] + gOvl11PairedValueLabelOffsets[secondBand][firstBand]);
    }
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) label, 0xAC, (s32) y);
}

void Ovl11DrawPairedValueBars(s32 *pair, s16 y) {
    s32 bottomY;
    s16 fillY;
    s16 x0;
    s16 x1;
    s16 y0;
    s32 edgeY0;
    s32 edgeY1;
    s32 labelOt;
    s32 renderOt;
    s32 bottomEdge;
    s32 edge;
    u32 *outlineOt;
    u32 *fillOt;

    renderOt = gActiveRenderContext->field_D8;
    edge = 0;
    outlineOt = (u32 *)(renderOt + 0x60);
    fillOt = (u32 *)(renderOt + 0x5C);
    labelOt = renderOt + 0x58;
    do {
        x0 = 0x28;
        if (edge % 3 != 0) {
            x0 = 0xA8;
        }
        edgeY0 = y;
        bottomEdge = edge & 2;
        bottomY = y + 8;
        if (bottomEdge != 0) {
            edgeY0 = bottomY;
        }
        y0 = edgeY0;
        x1 = 0xA8;
        if (bottomEdge != 0) {
            x1 = 0x28;
        }
        edgeY1 = y;
        if (edge % 3 != 0) {
            edgeY1 = bottomY;
        }
        DrawScreenLine(outlineOt, x0, y0, x1, edgeY1, 0xE0B888, 0);
        edge += 1;
    } while (edge < 4);
    fillY = y + 1;
    DrawScreenFlatRect((s32 *)outlineOt, 0x29, fillY, 0x80, 7, 0xC8A060, 0);
    DrawScreenLine(fillOt, 0x68, y, 0x68, bottomY, 0xE0B888, 0);
    x1 = ((pair[1] % 10000) * 64) / 10000;
    DrawScreenGradientRect((s32 *)fillOt, 0x69 - x1, fillY, x1, 7, 0xE8A0B8, 0xD14070, 0xE8A0B8, 0xD14070, 0);
    if (pair[1] / 10000 != 0) {
        DrawSpriteFrameRects(labelOt, (s32)D_8012CE88, 0xA, 0, 0x28, y - 0x18);
        ovl_11_func_800F7094(labelOt, 0x40, y - 0x20, pair[1] / 10000 - (pair[1] / 1000000) * 100);
    }
    DrawScreenGradientRect((s32 *)fillOt, 0x69, fillY, ((pair[0] % 10000) * 64) / 10000, 7, 0x4D70A0, 0xA6B8D0, 0x4D70A0, 0xA6B8D0, 0);
    if (pair[0] / 10000 != 0) {
        DrawSpriteFrameRects(labelOt, (s32)D_8012CE88, 0xA, 0, 0x68, y - 0x18);
        ovl_11_func_800F7094(labelOt, 0x80, y - 0x20, pair[0] / 10000 - (pair[0] / 1000000) * 100);
    }
}
