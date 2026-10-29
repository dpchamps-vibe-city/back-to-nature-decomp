/*
 * record_family_list — reconstructed translation unit.
 *
 * Scrolling four-row browser for tagged-record families, with private
 * cursor/scroll state, detail rows and a fixed-pitch segment gauge.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_record_family_list.h"
#include "screen_primitives.h"
#include "sound_commands.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "beveled_panel.h"
#include "ovl11_record_family_lookup.h"
#include "ui_sprites.h"
#include "psyq/libapi.h"
#include "psyq/libgpu.h"
#include "text_engine.h"

void Ovl11DrawRecordFamilyRow(Ovl11RecordFamilyRow *, s32, s16, s16);

s32 Ovl11DrawAndProcessRecordFamilyList(s16 requestedKind, s16 filter, s16 inputMode) {
    s32 input_mode;
    s32 result;
    Ovl11RecordFamilyRow *record;
    u16 terminator;
    s32 row_y;
    s16 icon;
    s16 kind;
    s32 ui;
    s32 row_offset;
    s32 next_index;
    s32 record_index;
    s32 header_ot;
    s32 row_ot;
    s32 count;
    s32 repeat_buttons;
    s32 pressed_buttons;
    s32 i;
    s32 prefix_value;
    s32 last_index;
    u8 *name;
    s32 text_index1;
    s32 text_index2;
    s32 text_index3;
    s32 text_index4;
    s32 next_row;

    input_mode = (s32) inputMode;
    if (inputMode == -1) {
        gOvl11RecordListCursor = 0;
        gOvl11RecordListScroll = 0;
        return -2;
    }
    ui = gActiveRenderContext->field_D8;
    header_ot = ui + 0x38;
    row_ot = ui + 0x40;
    kind = requestedKind >= 0 ? (requestedKind < 3 ? requestedKind : 2) : 0;
    result = -1;
    DrawScreenTileRect((u32 *)(gActiveRenderContext->field_D8 + 0x48), 0, 0, *(s16 *)&HWD0, *(s16 *)&VWD0, 0, 1);
    i = 0;
    DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x44), 1, 0x18, 0x20, 0x110, 0x74);
    DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x3C), 1, 0x50, 0x10, 0xA0, 0x20);
    for (;;) {
        prefix_value = ((Ovl11MenuGlobals *)D_8006C838)->text[i];
        terminator = prefix_value;
        if (terminator == 0xFFFF) {
            name = &gRecordFarmLabelTextAnchor[*gTextBankOffsets];
            (&gOvl11TextFormatScratch)[i] = ((u16 *)name)[0];
            text_index1 = i + 1;
            (&gOvl11TextFormatScratch)[text_index1] = ((u16 *)name)[1];
            text_index2 = i + 2;
            (&gOvl11TextFormatScratch)[text_index2] = ((u16 *)name)[2];
            text_index3 = i + 3;
            (&gOvl11TextFormatScratch)[text_index3] = ((u16 *)name)[3];
            text_index4 = i + 4;
            (&gOvl11TextFormatScratch)[text_index4] = terminator;
            break;
        }
        (&gOvl11TextFormatScratch)[i] = prefix_value;
        i++;
    }
    DrawTextLine(header_ot, (s32) &gOvl11TextFormatScratch, 0x52, 0x1A);
    gOvl11RecordListIconDrawers[kind](header_ot, 0xB0, 0x12);
    DrawTextLine(header_ot, (s32) &(gRecordFarmLabelTextAnchor + 0x3E)[*gTextBankOffsets], 0xC8, 0x1A);
    count = Ovl11FindFamilyRecordIndex(kind, filter, -1);
    if (input_mode == 0) {
        repeat_buttons = *(s32 *) ((u8 *) gPad0Input + 0);
        if (repeat_buttons & 0x1000) {
            PlaySoundEffect(5);
            gOvl11RecordListCursor--;
        } else if (repeat_buttons & 0x4000) {
            PlaySoundEffect(5);
            gOvl11RecordListCursor++;
        }
        last_index = count - 1;
        if ((s16) gOvl11RecordListCursor < 0) {
            gOvl11RecordListCursor = last_index;
        }
        if ((s32) last_index < (s16) gOvl11RecordListCursor) {
            gOvl11RecordListCursor = 0;
        }
        if (count >= 5) {
            if ((gOvl11RecordListScroll + 3) < (s16) gOvl11RecordListCursor) {
                gOvl11RecordListScroll = gOvl11RecordListCursor - 3;
            }
            if ((s16) gOvl11RecordListCursor < gOvl11RecordListScroll) {
                gOvl11RecordListScroll = (s16) gOvl11RecordListCursor;
            }
        }
    }
    if (gOvl11RecordListScroll > 0) {
        DrawAnimatedUiSprite29(row_ot, 0x120, 0x30);
    }
    if ((gOvl11RecordListScroll + 4) < count) {
        DrawAnimatedUiSprite28(row_ot, 0x120, 0x80);
    }
    DrawUiSprite0BFrame1(row_ot, 0x20, (s16) ((((s16) gOvl11RecordListCursor - gOvl11RecordListScroll) * 0x18) + 0x3C));
    for (i = 0; i < 4 && (next_row = i + 1, count >= gOvl11RecordListScroll + next_row); i++) {
        row_offset = i * 0x18;
        next_index = i + 1;
        record_index = Ovl11FindFamilyRecordIndex(kind, filter, (s16) ((u16) gOvl11RecordListScroll + i + 1));
        DrawNumberText((s16) ((u16) gOvl11RecordListScroll + next_index), 2, row_ot, 0x20, (s16) (s32) (s16) (row_offset + 0x36));
        row_y = row_offset + 0x30;
        if (kind == 0) {
            record = (Ovl11RecordFamilyRow *) ((u8 *) &gSmallTaggedRecordBase + (record_index * 0xB4));
        } else {
            record = (Ovl11RecordFamilyRow *) ((u8 *) &gTaggedRecordBase + (record_index * 0xB8));
        }
        icon = kind;
        if (kind == 2) {
            if (((Ovl11MenuGlobals *) ((u8 *) D_8006C838 + record_index * 0xB8))->status < 7U) {
                icon = 3;
            }
        }
        gOvl11RecordListIconDrawers[icon](row_ot, 0x40, row_y);
        Ovl11DrawRecordFamilyRow(record, row_ot, 0x58, row_y);
    }
    if (input_mode == 0) {
        pressed_buttons = *(s32 *) ((u8 *) gPad0Input + 8);
        if (pressed_buttons & 0x20) {
            PlaySoundEffect(1);
            result = -2;
        } else if (pressed_buttons & 0x40) {
            PlaySoundEffect(0);
            result = Ovl11FindFamilyRecordIndex(kind, filter, (s16) (gOvl11RecordListCursor + 1));
        }
    } else {
        result = Ovl11FindFamilyRecordIndex(kind, filter, (s16) (gOvl11RecordListCursor + 1));
    }
    return result;
}

s32 Ovl11ReadRecordRowValue(s16 *arg0);
void Ovl11DrawRecordRowGauge(s16 arg0, s32 arg1, s16 arg2, s16 arg3);

void Ovl11DrawRecordFamilyRow(Ovl11RecordFamilyRow *record, s32 orderingTable, s16 x, s16 y) {
    s16 valueX;
    s16 fill;
    s16 *text;
    s16 *cursor;
    u32 index;

    index = 0;
    fill = 0xFFD;
    text = &gOvl11TextFormatScratch;
    cursor = text;
    do {
        *cursor = fill;
        index += 1;
        cursor += 1;
    } while (index < 0x18U);
    if (record->flags & 0x40) {
        text[0] = 0x90;
    }
    text[4] = 0x71;
    WriteDecimalText(record->ordinalIndex + 1, text + 5, 2);
    text[8] = 0;
    text[9] = 0x26;
    text[10] = 0x24;
    *((u16 *) WriteDecimalText(Ovl11ReadRecordRowValue(&record->rowValue), text + 0xB, 2)) = 0xFFFF;
    DrawTextLine(orderingTable, (s32) ((u8 *) record + 4), (s32) (s16) (x + 2), (s32) y);
    /* The stored u16 is deliberately read as s16 before division. */
    Ovl11DrawRecordRowGauge((s16) ((s16) record->gaugeValue / 25), orderingTable, (s16) (x + 0x48), (s16) (y + 0xC));
    valueX = x + 0x58;
    DrawTextLine(orderingTable, (s32) text, (s32) valueX, (s32) y);
    DrawUiSprite2BFrame(orderingTable, (s16) (valueX + 0x10), y, record->trailingValue);
}

s32 Ovl11ReadRecordRowValue(s16 *value) {
    return *value;
}

void Ovl11DrawRecordRowGauge(s16 count, s32 orderingTable, s16 x, s16 y) {
    s16 segmentX;
    s32 remaining;
    s32 nextX;
    s32 fixedX;

    segmentX = x;
    if (count > 0) {
        remaining = count;
        /* Retain the 16.16 stepping accumulator and its halfword truncation. */
        fixedX = (segmentX << 0x10) + 0xFFF80000;
        do {
            DrawUiSprite2A(orderingTable, segmentX, y);
            nextX = fixedX >> 0x10;
            fixedX += 0xFFF80000;
            remaining -= 1;
            segmentX = (s16) nextX;
        } while (remaining != 0);
    }
}
