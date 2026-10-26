/*
 * debug_value_editors — reconstructed translation unit.
 *
 * Five debug edit pages for byte arrays, item fields and tool parameters,
 * plus their private value-step helper. See ovl30_debug_value_editors.h.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl30_debug_value_editors.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 Ovl30EditDayCount0(s32 handlerState) {
    u32 i;
    u32 index;
    struct Ovl30DayCounters *data;

    if (*(s32 *)gPad0Input & 0x2000) {
        gOvl30DayCount0Cursor += 10;
        if (gOvl30DayCount0Cursor >= 0x80) {
            gOvl30DayCount0Cursor = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x8000) {
        gOvl30DayCount0Cursor -= 10;
        if (gOvl30DayCount0Cursor <= 0) {
            gOvl30DayCount0Cursor = 0x7F;
        }
    }
    if (*(s32 *)gPad0Input & 0x4000) {
        gOvl30DayCount0Cursor++;
        if (gOvl30DayCount0Cursor >= 0x80) {
            gOvl30DayCount0Cursor = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x1000) {
        if (gOvl30DayCount0Cursor <= 0) {
            gOvl30DayCount0Cursor = 0x7F;
        } else {
            gOvl30DayCount0Cursor--;
        }
    }
    if (*(s32 *)gPad0Input & 0x20) {
        ((struct Ovl30DayCounters *)D_8006C838)->counters[gOvl30DayCount0Cursor]++;
        if (((struct Ovl30DayCounters *)D_8006C838)->counters[gOvl30DayCount0Cursor] >= 0xFF) {
            ((struct Ovl30DayCounters *)D_8006C838)->counters[gOvl30DayCount0Cursor] = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x40) {
        if (((struct Ovl30DayCounters *)D_8006C838)->counters[gOvl30DayCount0Cursor] == 0) {
            ((struct Ovl30DayCounters *)D_8006C838)->counters[gOvl30DayCount0Cursor] = 0xFF;
        } else {
            ((struct Ovl30DayCounters *)D_8006C838)->counters[gOvl30DayCount0Cursor]--;
        }
    }
    if (*(s32 *)gPad0Input & 0x10) {
        ((struct Ovl30DayCounters *)D_8006C838)->counters[gOvl30DayCount0Cursor] += 10;
        if (((struct Ovl30DayCounters *)D_8006C838)->counters[gOvl30DayCount0Cursor] >= 0xFF) {
            ((struct Ovl30DayCounters *)D_8006C838)->counters[gOvl30DayCount0Cursor] = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x80) {
        if (((struct Ovl30DayCounters *)D_8006C838)->counters[gOvl30DayCount0Cursor] == 0) {
            ((struct Ovl30DayCounters *)D_8006C838)->counters[gOvl30DayCount0Cursor] = 0xFF;
        } else {
            ((struct Ovl30DayCounters *)D_8006C838)->counters[gOvl30DayCount0Cursor] -= 10;
        }
    }
    data = (struct Ovl30DayCounters *)D_8006C838;
    FntPrint(gOvl30EditValueFormat, gOvl30DayCount0Cursor, data->counters[gOvl30DayCount0Cursor]);
    i = 0;
    do {
        index = gOvl30DayCount0Cursor + i;
        if (index < 0x80U) {
            FntPrint(gOvl30DayCount0Format, (s32)index, data->counters[index]);
        }
        i++;
    } while (i < 10U);
    return handlerState;
}

s32 Ovl30EditDayCount1(s32 handlerState) {
    u32 i;
    u32 index;
    struct Ovl30DayCounters1 *data;

    if (*(s32 *)gPad0Input & 0x2000) {
        gOvl30DayCount1Cursor += 10;
        if (gOvl30DayCount1Cursor >= 8) {
            gOvl30DayCount1Cursor = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x8000) {
        gOvl30DayCount1Cursor -= 10;
        if (gOvl30DayCount1Cursor <= 0) {
            gOvl30DayCount1Cursor = 7;
        }
    }
    if (*(s32 *)gPad0Input & 0x4000) {
        gOvl30DayCount1Cursor++;
        if (gOvl30DayCount1Cursor >= 8) {
            gOvl30DayCount1Cursor = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x1000) {
        if (gOvl30DayCount1Cursor <= 0) {
            gOvl30DayCount1Cursor = 7;
        } else {
            gOvl30DayCount1Cursor--;
        }
    }
    if (*(s32 *)gPad0Input & 0x20) {
        ((struct Ovl30DayCounters1 *)D_8006C838)->counters[gOvl30DayCount1Cursor]++;
        if (((struct Ovl30DayCounters1 *)D_8006C838)->counters[gOvl30DayCount1Cursor] >= 0xFF) {
            ((struct Ovl30DayCounters1 *)D_8006C838)->counters[gOvl30DayCount1Cursor] = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x40) {
        if (((struct Ovl30DayCounters1 *)D_8006C838)->counters[gOvl30DayCount1Cursor] == 0) {
            ((struct Ovl30DayCounters1 *)D_8006C838)->counters[gOvl30DayCount1Cursor] = 0xFF;
        } else {
            ((struct Ovl30DayCounters1 *)D_8006C838)->counters[gOvl30DayCount1Cursor]--;
        }
    }
    if (*(s32 *)gPad0Input & 0x10) {
        ((struct Ovl30DayCounters1 *)D_8006C838)->counters[gOvl30DayCount1Cursor] += 10;
        if (((struct Ovl30DayCounters1 *)D_8006C838)->counters[gOvl30DayCount1Cursor] >= 0xFF) {
            ((struct Ovl30DayCounters1 *)D_8006C838)->counters[gOvl30DayCount1Cursor] = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x80) {
        if (((struct Ovl30DayCounters1 *)D_8006C838)->counters[gOvl30DayCount1Cursor] == 0) {
            ((struct Ovl30DayCounters1 *)D_8006C838)->counters[gOvl30DayCount1Cursor] = 0xFF;
        } else {
            ((struct Ovl30DayCounters1 *)D_8006C838)->counters[gOvl30DayCount1Cursor] -= 10;
        }
    }
    data = (struct Ovl30DayCounters1 *)D_8006C838;
    FntPrint(gOvl30EditValueFormat, gOvl30DayCount1Cursor, data->counters[gOvl30DayCount1Cursor]);
    i = 0;
    do {
        index = gOvl30DayCount1Cursor + i;
        if (index < 8U) {
            FntPrint(gOvl30DayCount1Format, (s32)index, data->counters[index]);
        }
        i++;
    } while (i < 10U);
    return handlerState;
}

s32 Ovl30EditNValue(s32 handlerState) {
    u32 i;
    u32 index;
    struct Ovl30NValues *data;

    if (*(s32 *)gPad0Input & 0x2000) {
        gOvl30NValueCursor += 10;
        if (gOvl30NValueCursor >= 0x80) {
            gOvl30NValueCursor = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x8000) {
        gOvl30NValueCursor -= 10;
        if (gOvl30NValueCursor <= 0) {
            gOvl30NValueCursor = 0x7F;
        }
    }
    if (*(s32 *)gPad0Input & 0x4000) {
        gOvl30NValueCursor++;
        if (gOvl30NValueCursor >= 0x80) {
            gOvl30NValueCursor = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x1000) {
        if (gOvl30NValueCursor <= 0) {
            gOvl30NValueCursor = 0x7F;
        } else {
            gOvl30NValueCursor--;
        }
    }
    if (*(s32 *)gPad0Input & 0x20) {
        ((struct Ovl30NValues *)D_8006C838)->values[gOvl30NValueCursor]++;
        if (((struct Ovl30NValues *)D_8006C838)->values[gOvl30NValueCursor] >= 0xFF) {
            ((struct Ovl30NValues *)D_8006C838)->values[gOvl30NValueCursor] = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x40) {
        if (((struct Ovl30NValues *)D_8006C838)->values[gOvl30NValueCursor] == 0) {
            ((struct Ovl30NValues *)D_8006C838)->values[gOvl30NValueCursor] = 0xFF;
        } else {
            ((struct Ovl30NValues *)D_8006C838)->values[gOvl30NValueCursor]--;
        }
    }
    if (*(s32 *)gPad0Input & 0x10) {
        ((struct Ovl30NValues *)D_8006C838)->values[gOvl30NValueCursor] += 10;
        if (((struct Ovl30NValues *)D_8006C838)->values[gOvl30NValueCursor] >= 0xFF) {
            ((struct Ovl30NValues *)D_8006C838)->values[gOvl30NValueCursor] = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x80) {
        if (((struct Ovl30NValues *)D_8006C838)->values[gOvl30NValueCursor] == 0) {
            ((struct Ovl30NValues *)D_8006C838)->values[gOvl30NValueCursor] = 0xFF;
        } else {
            ((struct Ovl30NValues *)D_8006C838)->values[gOvl30NValueCursor] -= 10;
        }
    }
    data = (struct Ovl30NValues *)D_8006C838;
    FntPrint(gOvl30EditValueFormat, gOvl30NValueCursor, data->values[gOvl30NValueCursor]);
    i = 0;
    do {
        index = gOvl30NValueCursor + i;
        if (index < 0x80U) {
            FntPrint(gOvl30NValueFormat, (s32)index, data->values[index]);
        }
        i++;
    } while (i < 10U);
    return handlerState;
}

s32 Ovl30StepSelectedValue(s32 value) {
    s32 buttons = gPad0Input->repeatButtons;

    if (buttons & 0x80) {
        value -= 1;
    }
    if (buttons & 0x20) {
        value += 1;
    }
    return value;
}

s32 Ovl30StepSelectedValue(s32 value);

s32 Ovl30EditItemFields(s32 handlerState) {
    ItemData *item;

    if (gPad0Input->repeatButtons & 8) {
        gOvl30ItemIndex += 10;
        if (gOvl30ItemIndex >= 0x17B) {
            gOvl30ItemIndex = 0;
        }
    }
    if (gPad0Input->repeatButtons & 4) {
        gOvl30ItemIndex -= 10;
        if (gOvl30ItemIndex <= 0) {
            gOvl30ItemIndex = 0x17A;
        }
    }
    if (gPad0Input->repeatButtons & 0x2000) {
        gOvl30ItemIndex++;
        if (gOvl30ItemIndex >= 0x17B) {
            gOvl30ItemIndex = 0;
        }
    }
    if (gPad0Input->repeatButtons & 0x8000) {
        gOvl30ItemIndex--;
        if (gOvl30ItemIndex <= 0) {
            gOvl30ItemIndex = 0x17A;
        }
    }
    if (gPad0Input->repeatButtons & 0x1000) {
        gOvl30ItemFieldIndex--;
        if (gOvl30ItemFieldIndex < 0) {
            gOvl30ItemFieldIndex = 3;
        }
    }
    if (gPad0Input->repeatButtons & 0x4000) {
        gOvl30ItemFieldIndex++;
        if (gOvl30ItemFieldIndex >= 4) {
            gOvl30ItemFieldIndex = 0;
        }
    }
    item = &gItemDataTable[gOvl30ItemIndex];
    FntPrint(gOvl30EditorNewlines);
    FntPrint(gOvl30ItemIndexFormat, gOvl30ItemIndex);
    /* Existing debug labels identify these fields as BUY/SELL/LIFE/TIRED.
     * Keep the canonical ItemData views and their narrowing assignments. */
    if (gOvl30ItemFieldIndex == 0) {
        FntPrint(gOvl30FlagCursorMarker);
        item->field_10 = Ovl30StepSelectedValue(item->field_10);
    } else {
        FntPrint(gOvl30FlagIdleMarker);
    }
    FntPrint(gOvl30ItemBuyFormat, item->field_10);
    if (gOvl30ItemFieldIndex == 1) {
        FntPrint(gOvl30FlagCursorMarker);
        item->field_12 = Ovl30StepSelectedValue(item->field_12);
    } else {
        FntPrint(gOvl30FlagIdleMarker);
    }
    FntPrint(gOvl30ItemSellFormat, item->field_12);
    if (gOvl30ItemFieldIndex == 2) {
        FntPrint(gOvl30FlagCursorMarker);
        item->action.field_4 = Ovl30StepSelectedValue(item->action.field_4);
    } else {
        FntPrint(gOvl30FlagIdleMarker);
    }
    FntPrint(gOvl30ItemLifeFormat, item->action.field_4);
    if (gOvl30ItemFieldIndex == 3) {
        FntPrint(gOvl30FlagCursorMarker);
        item->action.field_5 = Ovl30StepSelectedValue(item->action.field_5);
    } else {
        FntPrint(gOvl30FlagIdleMarker);
    }
    FntPrint(gOvl30ItemTiredFormat, item->action.field_5);
    return handlerState;
}

s32 Ovl30EditToolParameters(s32 handlerState) {
    s32 column;
    s32 value;

    if (gPad0Input->repeatButtons & 0x2000) {
        gOvl30ToolIndex++;
        if (gOvl30ToolIndex >= 5) {
            gOvl30ToolIndex = 0;
        }
    }
    if (gPad0Input->repeatButtons & 0x8000) {
        gOvl30ToolIndex--;
        if (gOvl30ToolIndex < 0) {
            gOvl30ToolIndex = 4;
        }
    }
    if (gPad0Input->repeatButtons & 0x1000) {
        gOvl30ToolParameterIndex--;
        if (gOvl30ToolParameterIndex < 0) {
            gOvl30ToolParameterIndex = 4;
        }
    }
    if (gPad0Input->repeatButtons & 0x4000) {
        gOvl30ToolParameterIndex++;
        if (gOvl30ToolParameterIndex >= 5) {
            gOvl30ToolParameterIndex = 0;
        }
    }
    FntPrint(gOvl30EditorNewlines);
    FntPrint(gOvl30ToolIndexFormat, gOvl30ToolIndex, gOvl30ToolNames[gOvl30ToolIndex]);
    for (column = 0; column < 5; column++) {
        if (column == gOvl30ToolParameterIndex) {
            value = Ovl30StepSelectedValue(gOvl11ItemActionTierValues[gOvl30ToolIndex][column]);
            gOvl11ItemActionTierValues[gOvl30ToolIndex][column] = value;
            FntPrint(gOvl30FlagCursorMarker);
        } else {
            FntPrint(gOvl30FlagIdleMarker);
        }
        FntPrint(gOvl30ToolParameterFormat, gOvl11ItemActionTierValues[gOvl30ToolIndex][column]);
    }
    return handlerState;
}
