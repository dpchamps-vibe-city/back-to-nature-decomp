#include "common.h"
#include "text_engine.h"

/* Draw one quantity row. rowIndex selects the label and the stored/edited
 * values; all coordinates are pixels. The caller supplies a valid row index.
 * Selector 3 uses bank 2 and leaves the third row's label empty. */
void ovl_11_func_80104148(s32 rowIndex) {
    s16 storedValue;
    s16 editedValue;
    s16 y;
    s16 labelId;
    s32 bank;
    s32 mode;
    s32 three;
    register s16 x;
    register union {
        s32 pair;
        struct { signed int labelId : 16; signed int x : 16; } fields;
    } labelCell;
    s32 *labelText;
    s16 *numberText;
    s32 textContext;
    Ovl11CardView *state;

    bank = 2;
    rowIndex = (s16)rowIndex;
    textContext = gActiveRenderContext->field_D8 + 0x58;
    state = (Ovl11CardView *)D_8006C838;
    mode = state->selector;
    three = 3;
    if (mode != three) {
        bank = mode;
    }
    labelId = state->cards[bank][rowIndex][0];
    storedValue = state->cards[bank][rowIndex][5];
    editedValue = D_8012CF10[rowIndex];
    y = ((rowIndex + 1) * 14) + 50;
    if (mode == three && rowIndex == 2) {
        labelId = -1;
    }
    D_8012CF30_glyph.glyph = rowIndex < 2 ? rowIndex + 0x79 : rowIndex + 0x8F;
    D_8012CF30_glyph.terminator = 0xFFFF;
    x = 43;
    DrawTextLine(textContext, (s32)&D_8012CF30_glyph.glyph, x, (s32)y);
    /* Keep the label and starting column signed-halfword sized through the
     * label choice. The subsequent column advances have the same truncation. */
    labelCell.pair = (labelId & 0xFFFF) | (61 << 16);
    x = labelCell.fields.x;
    labelId = labelCell.fields.labelId;
    if (labelId != -1) {
        labelText = (s32 *)(D_800556A8 + D_80127430[labelId][0]);
    } else {
        labelText = &D_80075AD8;
    }
    DrawTextLine(textContext, (s32)labelText, x, (s32)y);
    x += 81;
    DrawTextLine(textContext, (s32)(D_80051BF4 + *gTextBankOffsets), x, (s32)y);
    x += 36;
    numberText = &D_8012CF30_glyph.glyph;
    *(u16 *)WriteDecimalText((s32)storedValue, numberText, 2) = 0xFFFF;
    DrawTextLine(textContext, (s32)numberText, x, (s32)y);
    x += 27;
    *(u16 *)WriteDecimalText((s32)editedValue, numberText, 2) = 0xFFFF;
    DrawTextLine(textContext, (s32)numberText, x, (s32)y);
    x += 18;
    DrawTextLine(textContext, (s32)(D_80051BF4 + 10 + *gTextBankOffsets), x, (s32)y);
}
