/*
 * Ten-row selection-list panel, with per-row text and formatted values.
 * Empty text pointers consume no vertical space. Row type 2 reserves five
 * additional number columns; types 0/2 append different fixed strings and
 * type 3 displays placeholder codes. Other types omit the number string.
 *
 * Original ovl_11 entries 8011D438/8011D474 compiled together: the child
 * receives the enclosing frame as GCC's static chain. A real nested
 * definition reproduces that ABI without CAPTURE_PREV_RET or an asm label.
 * Membership and conservative outer boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_selection_list_panel.h"
#include "psyq/stddef.h"
#include "beveled_panel.h"
#include "ovl11_value_panels.h"
#include "psyq/libgte.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_item_selection_value.h"

void func_80019E14(s32 arg0, s16 arg1, s16 arg2);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);

void Ovl11DrawSelectionListPanel(s32 *record, s16 x, s16 y) {
    s32 Ovl11HasRowTypeTwo(u8 *categories) {
        s32 index;
        u8 *category;

        category = categories;
        for (index = 0; index < 10; index++, category++) {
            if (*category == 2) {
                return 1;
            }
        }
        return 0;
    }

    s32 numberX;
    s16 *numberText;
    s32 *rowText;
    s32 *rowValues;
    s32 *rowTypes;
    s32 rowIndex;
    s32 fillCount;
    s32 columnSpan;
    s32 orderingTable;
    s32 drawnRows;
    s32 numberWidth;
    s32 fillIndex;
    u8 rowType;
    u8 textColumns;

    textColumns = *(u8 *) ((u8 *) record + 0x5A);
    orderingTable = gActiveRenderContext->field_D8 + 0x3C;
    numberX = (textColumns * 8) + 0x1C;
    columnSpan = (*(u8 *) ((u8 *) record + 0x5B)) - textColumns;
    numberWidth = Ovl11HasRowTypeTwo((u8 *) (record + 0xF));
    if (numberWidth == 0) {
        numberWidth = columnSpan - 2;
    } else {
        numberWidth = columnSpan - 7;
    }
    rowText = record;
    rowValues = record + 0xA;
    rowTypes = record + 0xF;
    drawnRows = 0;
    rowIndex = 0;
    do {
        if (*rowText != 0) {
            if (rowIndex == (*(s16 *) ((u8 *) record + 0x5C))) {
                func_80019E14(orderingTable, (s16) (x + 4), (s16) (y + (s16) ((drawnRows * 0xE) + 4)));
            }
            func_80017B3C(orderingTable, *rowText, (s16) (x + 0xC), (s32) (s16) (y + (s16) ((drawnRows * 0xE) + 4)));
            numberText = &gOvl11TextFormatScratch;
            rowType = *(u8 *) ((u8 *) rowTypes + 0);
            if (rowType == 0) {
                Ovl11CopyPrimaryValueLabel(func_8001A970(Ovl11GetItemPrice(*(u16 *) ((u8 *) rowValues + 0)), &gOvl11TextFormatScratch, numberWidth));
            } else if (rowType == 2) {
                Ovl11CopySecondaryValueLabel(func_8001A970(Ovl11LookupItemSelectionValue(*(u16 *) ((u8 *) rowValues + 0)), &gOvl11TextFormatScratch, numberWidth));
            } else {
                if (rowType == 3) {
                    fillCount = numberWidth + 1;
                    for (fillIndex = 0; fillIndex < fillCount; fillIndex++) {
                        (&gOvl11TextFormatScratch)[fillIndex] = 0x60;
                    }
                    {
                        s32 byteOffset;
                        byteOffset = fillIndex * 2;
                        *(u16 *) ((u8 *) &gOvl11TextFormatScratch + byteOffset) = 0xFFFF;
                    }
                } else {
                    numberText = NULL;
                }
            }
            if (numberText != NULL) {
                /* This column is absolute, not relative to panel origin x. */
                func_80017B3C(orderingTable, (s32) numberText, numberX, (s32) (s16) (y + (s16) ((drawnRows * 0xE) + 4)));
            }
            drawnRows++;
        }
        rowIndex++;
        rowText++;
        rowValues = (s32 *) ((u8 *) rowValues + 2);
        rowTypes = (s32 *) ((u8 *) rowTypes + 1);
    } while (rowIndex < 10);
    DrawBeveledPanel((u32 *) orderingTable, x, y, ((*(u8 *) ((u8 *) record + 0x5B)) * 8) + 0xF, (s16) (s32) (s16) ((drawnRows * 0xE) + 6));
}
