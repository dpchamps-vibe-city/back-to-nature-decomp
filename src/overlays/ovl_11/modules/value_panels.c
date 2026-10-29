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
#include "ui_sprites.h"
#include "psyq/libapi.h"
#include "psyq/libgpu.h"
#include "text_engine.h"

void Ovl11DrawValuePanelVariant(s32 wide);

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
        value = gSpendableBalance;
        labelOffset = 0x6A;
        copyLabel = Ovl11CopyPrimaryValueLabel;
        panelX = 0xE3;
        panelWidth = 0x4F;
        panelHeight = 0x22;
    }
    orderingTable = gActiveRenderContext->field_D8 + 0x40;
    copyLabel(WriteDecimalText(value, &gOvl11TextFormatScratch, 8));
    DrawBeveledPanel((u32 *)(gActiveRenderContext->field_D8 + 0x44), panelX, 0x10, panelWidth, panelHeight);
    /* These fixed x positions have bit 2 clear; preserve the original OR. */
    textX = panelX | 4;
    DrawTextLine(orderingTable, (s32)((void *)((u8 *)D_8005175C + (*D_80054BC0 + labelOffset))), textX, 0x14);
    DrawTextLine(orderingTable, (s32)&gOvl11TextFormatScratch, textX, panelHeight);
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
        DrawTextLine(orderingTable, prompt.unk0, (s16)headerX, (s32)rowY);
        heightAdjustment = 0;
        rowY += 0xE;
        leftMarkerX = headerX;
    }
    DrawBeveledPanel((u32 *)(gActiveRenderContext->field_D8 + 0x34), x, y, 0x74, (s16)(heightAdjustment + 0x30));
    *((u16 *)WriteDecimalText((s32)quantity, &gOvl11TextFormatScratch, 3)) = 0xFFFF;
    AppendU16String((u16 *)&gOvl11TextFormatScratch, (u16 *)prompt.unk4);
    DrawTextLine(orderingTable, (s32)&gOvl11TextFormatScratch, (s32)(s16)(x + 0x20), (s32)rowY);
    DrawAnimatedUiSprite0D(orderingTable, leftMarkerX, rowY);
    DrawAnimatedUiSprite0C(orderingTable, (s16)(x + 0x60), rowY);
    rowY += 0xE;
    Ovl11CopyPrimaryValueLabel(WriteDecimalText(value, &gOvl11TextFormatScratch, 5));
    DrawTextLine(gActiveRenderContext->field_D8 + 0x30, (s32)&gOvl11TextFormatScratch, (s32)(s16)(x + 0x10), (s32)rowY);
}

void Ovl11CopyPrimaryValueLabel(u16 *dst) {
    CopyTextString(dst, (u16 *)(D_80054BC0[0] + (s32)&gPrimaryValueLabelTextAnchor));
}

void Ovl11CopySecondaryValueLabel(u16 *dst) {
    CopyTextString(dst, (u16 *)(D_80054BC0[0] + (s32)&gSecondaryValueLabelTextAnchor));
}
