/*
 * Compact/wide value panels and quantity-prompt rows, ovl_11:8011CBC8..8011CF40.
 * Two fixed-label copy callbacks join the variant drawer and quantity drawer;
 * labels come from the runtime text bank, not new C string literals.
 * Uses the shared formatted-text scratch and current render context. See
 * ovl11_value_panels.h and notes/file-groupings.md for contracts and evidence.
 */
#include "common.h"
#include "ovl11_value_panels.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "beveled_panel.h"
#include "u16_string.h"

void Ovl11DrawValuePanelVariant(s32 wide);
s16 *func_8001A970(s32 value, s16 *dst, s32 width);
void func_80017B3C(s32 orderingTable, s32 text, s32 x, s32 y);
void func_80024810(s32 orderingTable, s16 x, s16 y);
void func_80024860(s32 orderingTable, s16 x, s16 y);
void func_8001ABF0(u16 *dst, u16 *src);

void Ovl11DrawCompactValuePanel(void) {
    Ovl11DrawValuePanelVariant(0);
}

void Ovl11DrawWideValuePanel(void) {
    Ovl11DrawValuePanelVariant(1);
}

void Ovl11DrawValuePanelVariant(s32 wide) {
    s16 panelX;
    s16 panelWidth;
    s32 orderingTable;
    s32 textX;
    s32 value;
    s32 labelOffset;
    s16 panelHeight;
    void (*copyLabel)(u16 *);

    if (wide != 0) {
        value = gSelectedThresholdBalance;
        labelOffset = 0x76;
        copyLabel = Ovl11CopySecondaryValueLabel;
        panelX = 0xBB;
        panelWidth = 0x77;
        panelHeight = 0x22;
    } else {
        value = D_80071A5C;
        labelOffset = 0x6A;
        copyLabel = Ovl11CopyPrimaryValueLabel;
        panelX = 0xE3;
        panelWidth = 0x4F;
        panelHeight = 0x22;
    }
    orderingTable = gActiveRenderContext->field_D8 + 0x40;
    copyLabel(func_8001A970(value, &gOvl11TextFormatScratch, 8));
    DrawBeveledPanel((u32 *)(gActiveRenderContext->field_D8 + 0x44), panelX, 0x10, panelWidth, panelHeight);
    /* These fixed x positions have bit 2 clear; preserve the original OR. */
    textX = panelX | 4;
    func_80017B3C(orderingTable, (s32)((void *)((u8 *)D_8005175C + (*D_80054BC0 + labelOffset))), textX, 0x14);
    func_80017B3C(orderingTable, (s32)&gOvl11TextFormatScratch, textX, panelHeight);
}

void Ovl11DrawQuantityPromptRows(Ovl11D8012D538 prompt, s16 quantity, s32 value, s16 x, s16 y) {
    s32 headerX;
    s16 rowY;
    s32 leftMarkerX;
    s32 orderingTable;
    s32 heightAdjustment;

    rowY = y + 4;
    orderingTable = gActiveRenderContext->field_D8 + 0x30;
    if (prompt.unk0 == 0) {
        heightAdjustment = -0xE;
        leftMarkerX = x + 4;
    } else {
        headerX = x + 4;
        func_80017B3C(orderingTable, prompt.unk0, (s16)headerX, (s32)rowY);
        heightAdjustment = 0;
        rowY += 0xE;
        leftMarkerX = headerX;
    }
    DrawBeveledPanel((u32 *)(gActiveRenderContext->field_D8 + 0x34), x, y, 0x74, (s16)(heightAdjustment + 0x30));
    *((u16 *)func_8001A970((s32)quantity, &gOvl11TextFormatScratch, 3)) = 0xFFFF;
    AppendU16String((u16 *)&gOvl11TextFormatScratch, (u16 *)prompt.unk4);
    func_80017B3C(orderingTable, (s32)&gOvl11TextFormatScratch, (s32)(s16)(x + 0x20), (s32)rowY);
    func_80024810(orderingTable, leftMarkerX, rowY);
    func_80024860(orderingTable, (s16)(x + 0x60), rowY);
    rowY += 0xE;
    Ovl11CopyPrimaryValueLabel(func_8001A970(value, &gOvl11TextFormatScratch, 5));
    func_80017B3C(gActiveRenderContext->field_D8 + 0x30, (s32)&gOvl11TextFormatScratch, (s32)(s16)(x + 0x10), (s32)rowY);
}

void Ovl11CopyPrimaryValueLabel(u16 *dst) {
    func_8001ABF0(dst, (u16 *)(D_80054BC0[0] + (s32)&gPrimaryValueLabelTextAnchor));
}

void Ovl11CopySecondaryValueLabel(u16 *dst) {
    func_8001ABF0(dst, (u16 *)(D_80054BC0[0] + (s32)&gSecondaryValueLabelTextAnchor));
}
