/*
 * quantity_editor — reconstructed translation unit.
 *
 * Six-row quantity-editing dialog. The selector chooses one of three banks (3
 * maps to bank 2); editor state and the selected row drive selection, bounded
 * quantity changes, preview rendering and guarded commit/cancel paths.
 * Quantity changes transfer 50 accumulator units per item. External callers
 * initialize or service the dialog and test its state; helpers retain
 * original linkage and address order.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_quantity_editor.h"
#include "game_types.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "screen_primitives.h"
#include "message_window.h"
#include "ovl11_menu_exit_flags.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "beveled_panel.h"
#include "psyq/libapi.h"
#include "psyq/libgpu.h"
#include "text_engine.h"
#include "ui_sprites.h"

s32 Ovl11IsSelectedQuantitySumZero(void) {
    Ovl11RowE522View *v;
    u16 sel;
    s32 i;
    s32 sum;

    v = (Ovl11RowE522View *)&D_8006C838;
    sum = 0;
    sel = 2;
    if (v->selector != 3) {
        sel = (u16) v->selector;
    }
    for (i = 0; i < 6; i++) {
        sum += v->rows[sel][i][0];
    }
    return sum == 0;
}

s32 Ovl11IsQuantityEditorInactive(void) {
    return D_80127428 == 0;
}

void Ovl11ResetQuantityEditor(void) {
    s32 i;

    D_80127428 = 0;
    gOvl11QuantityEditorSelectedRow = 0;
    for (i = 0; i < 6; i++) {
        D_8012CF10[i] = 0;
    }
    D_8012CF24 = 0;
    D_8012CF1C = 0;
}

void Ovl11BeginQuantityEditor(void) {
    Ovl11QuantityWorkView *v;
    s16 sel;
    s32 cnt;

    v = (Ovl11QuantityWorkView *)&D_8006C838;
    sel = 2;
    if (v->unkE514 != 3) {
        sel = v->unkE514;
    }
    Ovl11ResetQuantityEditor();
    D_80127428 = 1;
    PlaySoundEffect(3);
    gOvl11QuantityEditorBalance = v->field_5224;
    cnt = 0;
    do {
        D_8012CF10[cnt] = (u16)v->rows[sel][cnt][0];
        cnt += 1;
    } while (cnt < 6);
}

void Ovl11ServiceQuantityEditor(void);

void Ovl11UpdateQuantityEditor(void) {
    if (D_80127428 != 0) {
        SetTextSpacing(0, 2);
        DrawScreenTileRect((u32 *)(gActiveRenderContext->field_D8 + 0x60), 0, 0, (s16)HWD0, (s16)VWD0, 0, 1);
        Ovl11ServiceQuantityEditor();
        SetTextSpacing(0, 0);
    }
}

void Ovl11DrawQuantityEditorBalance(void);
void Ovl11DrawQuantityEditorRows(void);
void Ovl11DrawQuantityEditorCursor(void);
s32 Ovl11HandleQuantitySelectionInput(void);
s32 Ovl11HandleQuantityEditInput(void);
void Ovl11CommitQuantityEditor(void);

void Ovl11ServiceQuantityEditor(void) {
    s32 result;

    Ovl11DrawQuantityEditorBalance();
    Ovl11DrawQuantityEditorRows();
    Ovl11DrawQuantityEditorCursor();
    if (D_80127428 == 1) {
        result = Ovl11HandleQuantitySelectionInput();
        if (result == -1) {
            CloseMessageWindow();
            D_80127428 = 4;
        } else if (result == 1) {
            D_80127428 = 3;
        } else if (result == 2) {
            D_80127428 = result;
        }
    } else if (D_80127428 == 2) {
        result = Ovl11HandleQuantityEditInput();
        if (result == -1) {
            D_80127428 = 1;
            D_8012CF10[gOvl11QuantityEditorSelectedRow] = D_8012CF1C;
            gOvl11QuantityEditorBalance = D_8012CF24;
        } else if (result == -2) {
            CloseMessageWindow();
            D_80127428 = 4;
        } else if (result == 1) {
            D_80127428 = result;
        } else if (result == 2) {
            CloseMessageWindow();
            D_80127428 = 3;
        }
    } else if (D_80127428 == 3) {
        CloseMessageWindow();
        RequestMessageWindow(1, 0x448);
        Ovl11CommitQuantityEditor();
        D_80127428 = 5;
        return;
    } else if (D_80127428 == 4) {
        CloseMessageWindow();
        RequestMessageWindow(1, 0x447);
        Ovl11CommitQuantityEditor();
        D_80127428 = 5;
        return;
    } else if (D_80127428 == 5) {
        if (GetMessageWindowStatus() == 2) {
            Ovl11PostMenuExitFlags();
            PlaySoundEffect(3);
            D_80127428 = 0;
        }
        return;
    }
    SetMessagePortraitEnabled(0);
    SetTextInputEnabled(0);
    RequestMessageWindow(1, 0x764);
}

void Ovl11CommitQuantityEditor(void) {
    Ovl11QuantityWorkView *v;
    Ovl11QuantityWorkView *p;
    Ovl11QuantityWorkView *saved;
    Ovl11QuantityWorkView *w;
    s32 sel;
    s32 delta;
    s32 i;
    s32 three;
    s16 e;

    sel = 2;
    v = (Ovl11QuantityWorkView *)&D_8006C838;
    saved = v;
    e = v->unkE514;
    three = 3;
    if (e != three) {
        sel = e;
    }
    if (D_80127428 != three) {
        return;
    }
    p = v;
    v = saved;
    delta = v->field_5224 - gOvl11QuantityEditorBalance;
    if (delta > 0) {
        p->field_49C8 += delta;
    } else if (delta < 0) {
        p->field_49C8 += delta;
    }
    w = (Ovl11QuantityWorkView *)&D_8006C838;
    w->field_5224 = gOvl11QuantityEditorBalance;
    for (i = 0; i < 6; i++) {
        w->rows[sel][i][0] = (u16)D_8012CF10[i];
    }
}

void Ovl11AdjustEditedQuantity (s16 arg0, s16 *arg1);
s32 Ovl11AreEditedQuantitiesUnchanged (void);

s32 Ovl11HandleQuantityEditInput(void) {
    s16 *temp_s1;
    s16 var_s0;
    s32 temp_a1;
    s32 temp_a2;

    if (IsMessageWindowStatusFinal() == 0) {
        return 0;
    }
    var_s0 = 0;
    temp_s1 = &D_8012CF10[gOvl11QuantityEditorSelectedRow];
    temp_a2 = *(s32 *) ((u8 *) gPad0Input + 0);
    if (temp_a2 & 0x1000) {
        var_s0 = 0xA;
    } else if (temp_a2 & 0x4000) {
        var_s0 = -0xA;
    } else if (temp_a2 & 0x2000) {
        var_s0 = 1;
    } else if (temp_a2 & 0x8000) {
        var_s0 = -1;
    }
    if (var_s0 != 0) {
        PlaySoundEffect(5);
        Ovl11AdjustEditedQuantity(var_s0, temp_s1);
    }
    temp_a1 = *(s32 *) ((u8 *) gPad0Input + 8);
    if (temp_a1 & 0x800) {
        if (Ovl11AreEditedQuantitiesUnchanged() == 1) {
            PlaySoundEffect(1);
            return -2;
        }
        PlaySoundEffect(0);
        return 2;
    }
    if (temp_a1 & 0x20) {
        PlaySoundEffect(1);
        return -1;
    }
    if (temp_a1 & 0x40) {
        PlaySoundEffect(0);
        return 1;
    }
    return 0;
}

s32 Ovl11AreEditedQuantitiesUnchanged(void);

s32 Ovl11HandleQuantitySelectionInput(void) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_v0;
    s32 var_s0;

    if (IsMessageWindowStatusFinal() != 0) {
        var_s0 = 0;
        temp_a0 = *(s32 *) ((u8 *) gPad0Input + 0);
        if (temp_a0 & 0x1000) {
            var_s0 = -1;
        } else if (temp_a0 & 0x4000) {
            var_s0 = 1;
        }
        if (var_s0 != 0) {
            PlaySoundEffect(5);
            temp_v0 = gOvl11QuantityEditorSelectedRow + var_s0;
            gOvl11QuantityEditorSelectedRow = temp_v0;
            if (temp_v0 < 0) {
                gOvl11QuantityEditorSelectedRow = 6;
            }
            if (gOvl11QuantityEditorSelectedRow >= 7) {
                gOvl11QuantityEditorSelectedRow = 0;
            }
        }
        temp_a0_2 = *(s32 *) ((u8 *) gPad0Input + 8);
        if (temp_a0_2 & 0x800) {
            if (Ovl11AreEditedQuantitiesUnchanged() == 1) {
                goto block_18;
            }
            goto block_19;
        }
        if (!(temp_a0_2 & 0x20)) {
            if (temp_a0_2 & 0x40) {
                if (gOvl11QuantityEditorSelectedRow != 6) {
                    D_8012CF1C = D_8012CF10[gOvl11QuantityEditorSelectedRow];
                    D_8012CF24 = gOvl11QuantityEditorBalance;
                    PlaySoundEffect(0);
                    return 2;
                }
                if (Ovl11AreEditedQuantitiesUnchanged() != 1) {
                    goto block_19;
                }
            } else {
                goto block_20;
            }
        }
block_18:
        PlaySoundEffect(1);
        return -1;
block_19:
        PlaySoundEffect(0);
        return 1;
    }
block_20:
    return 0;
}

void Ovl11DrawQuantityEditorBalance(void) {
    s32 temp_s3;
    s32 temp_s3_2;

    temp_s3 = gActiveRenderContext->field_D8;
    temp_s3_2 = temp_s3 + 0x58;
    DrawMenuPanel((u32 *)(temp_s3 + 0x5C), 0, 0xE3, 0x10, 0x47, 0x1E);
    DrawTextLine(temp_s3_2, (s32)((u8 *)gQuantityEditorMoneyTextAnchor + gTextBankOffsets[1]), 0xE5, 0x12);
    CopyTextString(WriteDecimalText(gOvl11QuantityEditorBalance, D_8012CF30, 7),
                  (u16 *)((u8 *)gQuantityEditorMoneyTextAnchor + 0x42 + gTextBankOffsets[1]));
    DrawTextLine(temp_s3_2, (s32)D_8012CF30, 0xE5, 0x20);
}

void Ovl11DrawQuantityEditorRow (s32 arg0);

void Ovl11DrawQuantityEditorRows(void) {
    s32 var_s2;
    s32 temp_s3;
    s32 temp_t0;
    u16 var_a1;

    temp_t0 = gActiveRenderContext->field_D8;
    temp_s3 = temp_t0 + 0x58;
    DrawMenuPanel((u32 *)(temp_t0 + 0x5C), 0, 0x20, 0x30, 0x101, 0x72);
    var_a1 = 2;
    if (((Ovl11HorseRaceView *) D_8006C838)->selector != 3) {
        var_a1 = (u16) ((Ovl11HorseRaceView *) D_8006C838)->selector;
    }
    *(u16 *)WriteDecimalText(var_a1 + 1, D_8012CF30, 1) = 0xFFFF;
    SetTextSourcePointer(0, (s32) D_8012CF30);
    DrawTextLine(temp_s3, (s32)(D_80051C0E + gTextBankOffsets[0]), 0x22, 0x32);
    DrawTextLine(temp_s3, (s32)(D_80051C0E + 0x18 + gTextBankOffsets[0]), 0xDF, 0x94);
    var_s2 = 0;
    do {
        Ovl11DrawQuantityEditorRow((s16) var_s2);
        var_s2 += 1;
    } while (var_s2 < 6);
}

void Ovl11DrawQuantityEditorCursor(void) {
    s16 var_a2;
    s32 var_a1;

    var_a2 = (gOvl11QuantityEditorSelectedRow + 1) * 0xE + 0x32;
    var_a1 = -1;
    if (D_80127428 == 1) {
        var_a1 = 0xD6;
        if (gOvl11QuantityEditorSelectedRow != 6) {
            var_a1 = 0x22;
        }
    } else if (D_80127428 == 2) {
        var_a1 = 0xC4;
    }
    if (var_a1 != -1) {
        DrawUiSprite0BFrame1(gActiveRenderContext->field_D8 + 0x54, var_a1 + 6, var_a2 + 6);
    }
}

/* Draw one quantity row. rowIndex selects the label and the stored/edited
 * values; all coordinates are pixels. The caller supplies a valid row index.
 * Selector 3 uses bank 2 and leaves the third row's label empty. */
void Ovl11DrawQuantityEditorRow(s32 rowIndex) {
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
    Ovl11HorseRaceView *state;
    Ovl11HorseRaceUpperView *upper;

    bank = 2;
    rowIndex = (s16)rowIndex;
    textContext = gActiveRenderContext->field_D8 + 0x58;
    state = (Ovl11HorseRaceView *)D_8006C838;
    mode = state->selector;
    /* Reuse the upper work-area base through the bank-selection branch. */
    upper = (Ovl11HorseRaceUpperView *)((u8 *)state + 0x8000);
    three = 3;
    if (mode != three) {
        bank = mode;
    }
    labelId = upper->entries[bank][rowIndex][0];
    storedValue = upper->entries[bank][rowIndex][5];
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

s32 Ovl11AreEditedQuantitiesUnchanged(void) {
    Ovl11QuantityWorkView *v;
    s32 sel;
    s32 cnt;
    s32 res;

    v = (Ovl11QuantityWorkView *)&D_8006C838;
    sel = 2;
    if (v->unkE514 != 3) {
        sel = v->unkE514;
    }
    res = 1;
    cnt = 0;
    do {
        if (D_8012CF10[cnt] != v->rows[sel][cnt][0]) {
            res = 0;
        }
        cnt += 1;
    } while (cnt < 6);
    return res;
}

void Ovl11AdjustEditedQuantity(s16 arg0, s16 *arg1) {
    s32 old;
    s32 limit;
    s32 value;

    old = *arg1;
    limit = old + gOvl11QuantityEditorBalance / 50;
    if (limit >= 100) {
        limit = 99;
    }
    if (limit < 0) {
        limit = 0;
    }
    value = old + arg0;
    if (value < 0) {
        if (old == 0) {
            *arg1 = limit;
        } else {
            *arg1 = 0;
        }
    } else if (limit < value) {
        if (old == limit) {
            *arg1 = 0;
        } else {
            *arg1 = limit;
        }
    } else {
        *arg1 = value;
    }

    gOvl11QuantityEditorBalance += old * 50;
    gOvl11QuantityEditorBalance -= *arg1 * 50;
}
