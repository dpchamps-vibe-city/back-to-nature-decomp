/*
 * Phase-selected item-slot screen, ovl_11:800F787C-800F8CF8.
 *
 * The frame driver owns the input -> record operation -> navigation/message
 * sequence and the ordering-table layers used by its seven view helpers.
 * Slot 0 is the selected record, 1..8 the snapshot row, and 9..16 the visible
 * eight-cell page. The same layout selector controls the snapshot count and
 * the navigation links. Records are three halfwords: ID, attribute, quantity.
 * Texture ownership changes before moves/swaps; stack merges leave quantities
 * above 99 in the source. The public interface is ovl11_record_slot_menu.h.
 *
 * All view callers are inside this unit; helpers retain external linkage.
 * Membership and evidence-limited outer boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_texture_cell_cache.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"
#include "integer_power.h"
#include "ovl11_selection_texture_transfer.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "packet_work.h"
#include "beveled_panel.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "ovl11_slot_sprite_cache.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_item_stack_clear.h"
#include "ovl11_slot_decorations.h"
#include "ovl11_cell_grid_lookup.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "ovl11_record_slot_menu.h"

void func_80017A38(s16 arg0, s16 arg1);
void SetVal8005E2BC(s32 arg0);
s32 Ovl11HandleMenuGridInput(void);
void Ovl11UpdateMenuGridNavigation(s32 slotCount);
void Ovl11UpdateMenuGridMessage(s32 textDisplay, s32 panelDisplay, s32 slot, s32 page);
void Ovl11DrawMenuGridSlots(s32 *orderingTable, s32 firstSlot, s32 count, s16 *records);
void Ovl11DrawMenuGridSlot(s32 *orderingTable, s32 slot, s16 *record);
void Ovl11DrawMenuGridPageCount(s32 textDisplay, s32 slotCount);
void Ovl11DrawMenuGridPanels(u32 *panelOrderingTable, s32 textDisplay,
                           s32 topTextAddress, s32 bottomTextAddress);

void Ovl11UpdateMenuGridScreen(void) {
    s32 firstText;
    s32 secondText;
    s32 message;
    s32 items;
    s32 selected;
    s32 snapshot;
    s32 object;
    s32 text;
    u32 *panel;
    s32 *display;

    func_80017A38(0, 2);
    if (IsMessageWindowStatusFinal() && (gPad0Input->pressedButtons & 0x800)) {
        gOvl11InteractionPhase = 0xFE;
        CloseMessageWindow();
        PlaySoundEffect(3);
        return;
    }
    firstText = gOvl11InteractionPhase;
    if (firstText == 0) {
        firstText = 0x448;
        secondText = 0x466;
        message = 0x3AD;
        items = (s32)D_8006C838 + 0x468A;
        selected = (s32)D_8006C838 + 0x524C;
        snapshot = (s32)D_8006C838 + 0x5258;
    } else if (firstText == 2) {
        firstText = 0x454;
        secondText = 0x476;
        message = 0x3AE;
        items = (s32)D_8006C838 + 0x450A;
        selected = (s32)D_8006C838 + 0x5252;
        snapshot = (s32)D_8006C838 + 0x5288;
    } else if (firstText == 1) {
        firstText = 0x454;
        secondText = 0x484;
        message = 0x3AF;
        items = (s32)D_8006C838 + 0x480A;
        selected = (s32)D_8006C838 + 0x5252;
        snapshot = (s32)D_8006C838 + 0x5288;
    } else {
        return;
    }
    firstText = (s32)((u8 *)D_8005175C + (gTextBankOffsets[0] + firstText));
    secondText = (s32)((u8 *)D_8005175C + (gTextBankOffsets[0] + secondText));
    object = gActiveRenderContext->field_D8;
    panel = (u32 *)(object + 0x68);
    text = object + 0x64;
    display = (s32 *)(object + 0x5C);
    Ovl11DrawMenuGridPanels(panel, text, firstText, secondText);
    Ovl11DrawMenuGridPageCount(text, 0x40);
    Ovl11HandleMenuGridInput();
    Ovl11DrawMenuGridSlot(display, 0, (s16 *)selected);
    Ovl11DrawMenuGridSlots(display, 1, pow_int(2, gMenuGridLayoutIndex + 1), (s16 *)snapshot);
    Ovl11UpdateMenuGridNavigation(0x40);
    items += gOvl11MenuGridPageIndex * 48;
    Ovl11DrawMenuGridSlots(display, 9, 8, (s16 *)items);
    Ovl11UpdateMenuGridMessage(text, (s32)panel, gOvl11MenuGridCursorSlot, gOvl11MenuGridPageIndex);
    if (gOvl11MenuGridMessageState == 0xFF) {
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(3, message);
    }
}

s32 Ovl11ApplySlotRecordOperation(s32 sourceSlot, s32 destinationSlot,
                                s32 sourcePage, s32 destinationPage);
void Ovl11CompactItemSlots(s16 *arg0, s16 arg1);

s32 Ovl11HandleMenuGridInput(void) {
    s16 temp_s0;
    s32 temp_s0_2;
    s32 temp_v1;

    temp_s0 = gOvl11MenuGridMessageState;
    if ((temp_s0 == 0xFF) && (IsMessageWindowStatusFinal() != 0)) {
        temp_v1 = gPad0Input->pressedButtons;
        if (temp_v1 & 0x10) {
            if (*Ovl11GetItemSlotRecord((s32) gOvl11MenuGridCursorSlot, (s32) gOvl11MenuGridPageIndex) != 0) {
                CloseMessageWindow();
                PlaySoundEffect(0);
                gOvl11MenuGridMessageState = 0;
                goto block_32;
            }
            PlaySoundEffect(2);
            return 0;
        }
        if (temp_v1 & 0x20) {
            if (gOvl11MenuGridSelectedSlot == temp_s0) {
                CloseMessageWindow();
                gOvl11InteractionPhase = 0xFE;
                PlaySoundEffect(3);
                return 0;
            }
            PlaySoundEffect(1);
            gOvl11MenuGridSelectedSlot = temp_s0;
            goto block_32;
        }
        if (temp_v1 & 0x40) {
            if (gOvl11MenuGridSelectedSlot == temp_s0) {
                PlaySoundEffect(0);
                gOvl11MenuGridSelectedSlot = (s16) (u16) gOvl11MenuGridCursorSlot;
                gOvl11MenuGridSelectedPageIndex = (s16) (u16) gOvl11MenuGridPageIndex;
                goto block_32;
            }
            gOvl11MenuGridSavedCursorSlot = (u16) gOvl11MenuGridCursorSlot;
            if ((gOvl11MenuGridSelectedSlot == (s16) (u16) gOvl11MenuGridCursorSlot) && ((gOvl11MenuGridSelectedPageIndex == gOvl11MenuGridPageIndex) || (gOvl11MenuGridSelectedSlot < 9))) {
                gOvl11MenuGridCursorSlot = 0;
            }
            Ovl11ReleaseUnusedSelectionTextures();
            temp_s0_2 = Ovl11ApplySlotRecordOperation((s32) gOvl11MenuGridSelectedSlot, (s32) gOvl11MenuGridCursorSlot, (s32) gOvl11MenuGridSelectedPageIndex, (s32) gOvl11MenuGridPageIndex);
            Ovl11ReleaseUnusedSelectionTextures();
            gOvl11MenuGridCursorSlot = (s16) gOvl11MenuGridSavedCursorSlot;
            if (temp_s0_2 == 1) {
                PlaySoundEffect(0);
                gOvl11MenuGridSelectedSlot = 0xFF;
                return 1;
            }
            PlaySoundEffect(2);
            return 0;
        }
        if (temp_v1 & 0x100) {
            if (gOvl11MenuGridSelectedSlot == temp_s0) {
                PlaySoundEffect(0);
                switch (gOvl11InteractionPhase) {
                case 0:
                    Ovl11CompactItemSlots((s16 *) &gItemOverflowSlotBase, 0x40);
                    break;
                case 2:
                    Ovl11CompactItemSlots((s16 *) D_80070D42_slots, 0x40);
                    break;
                case 1:
                    Ovl11CompactItemSlots((s16 *) D_80071042, 0x40);
                    break;
                }
                goto block_32;
            }
            PlaySoundEffect(2);
            goto block_32;
        }
        goto block_32;
    }
block_32:
    return 0;
}

s32 Ovl11TakeSlotRecord(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3);
s32 Ovl11SwapSlotRecords(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3);
s32 Ovl11MergeSlotRecords(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3);

/* Empty endpoints move a record; different IDs swap; equal IDs try to merge.
 * Selecting the same slot/page is a no-op, unlike two different empty slots. */
s32 Ovl11ApplySlotRecordOperation(s32 sourceSlot, s32 destinationSlot,
                                s32 sourcePage, s32 destinationPage) {
    s32 result;
    s16 *destination;
    s16 *source;

    if (sourceSlot != destinationSlot || sourcePage != destinationPage) {
        source = Ovl11GetItemSlotRecord(sourceSlot, sourcePage);
        destination = Ovl11GetItemSlotRecord(destinationSlot, destinationPage);
        if (*source == 0 && *destination == 0) {
            return 1;
        }
        if (*destination == 0) {
            return Ovl11TakeSlotRecord((u16 *)source, sourceSlot, (u16 *)destination, destinationSlot);
        }
        if (*source == 0) {
            return Ovl11TakeSlotRecord((u16 *)destination, destinationSlot, (u16 *)source, sourceSlot);
        }
        if (*source != *destination) {
            return Ovl11SwapSlotRecords((u16 *)source, sourceSlot, (u16 *)destination, destinationSlot);
        }
        result = Ovl11MergeSlotRecords((u16 *)source, sourceSlot, (u16 *)destination, destinationSlot);
        return result;
    }
    return 0;
}

void Ovl11SwapCopyRecords(Ovl11CopyRecord *arg0, Ovl11CopyRecord *arg1);
void Ovl11CopySlotRecord(Ovl11CopyRecord *arg0, Ovl11CopyRecord *arg1);

s32 Ovl11TakeSlotRecord(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3) {
    s32 temp_v0;
    s32 var_v0;
    s8 temp_v0_2;

    temp_v0 = Ovl11GetItemSlotLevel(arg3);
    if (temp_v0 != 1) {
        if (temp_v0 < 2) {
            if (temp_v0 != 0) {
                return 0;
            }
            goto block_9;
        }
        var_v0 = 0;
        if (temp_v0 == 2) {
            var_v0 = 0;
            if (Ovl11IsItemStatusNegative((s16) arg0[0]) != 0) {
                var_v0 = 0;
                if (gOvl11InteractionPhase != Ovl11ClassifyItemStatus((s16) arg0[0])) {
                    return 0;
                }
                Ovl11ReorderSelectionTextures(arg0, arg1, arg2, arg3, 0);
                Ovl11SwapCopyRecords((Ovl11CopyRecord *) arg0, (Ovl11CopyRecord *) arg2);
                return 1;
            }
        }
        /* Duplicate return node #17. Try simplifying control flow for better match */
        return var_v0;
    }
    var_v0 = 0;
    if (Ovl11IsItemStatusNegative((s16) arg0[0]) != 0) {
block_9:
        temp_v0_2 = Ovl11GetItemType((s16) arg0[0]);
        if (temp_v0_2 != 0) {
            if (temp_v0_2 >= 0) {
                var_v0 = 0;
                if (temp_v0_2 < 3) {
                    Ovl11ReorderSelectionTextures(arg0, arg1, arg2, arg3, 0);
                    Ovl11SwapCopyRecords((Ovl11CopyRecord *) arg0, (Ovl11CopyRecord *) arg2);
                    return 1;
                }
                /* Duplicate return node #17. Try simplifying control flow for better match */
                return var_v0;
            }
            var_v0 = 0;
            /* Duplicate return node #17. Try simplifying control flow for better match */
            return var_v0;
        }
        Ovl11ReorderSelectionTextures(arg0, arg1, arg2, arg3, 0);
        Ovl11CopySlotRecord((Ovl11CopyRecord *) arg0, (Ovl11CopyRecord *) arg2);
        (*(s16 *) ((u8 *) arg2 + 4)) = 1;
        var_v0 = 1;
        if (--((s16 *) arg0)[2] <= 0) {
            Ovl11ClearItemStackEntry((s16 *) arg0);
            return 1;
        }
        /* Duplicate return node #17. Try simplifying control flow for better match */
        return var_v0;
    }
    return var_v0;
}

s32 Ovl11SwapSlotRecords(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3) {
    switch (Ovl11GetItemSlotLevel(arg3)) {
    case 2:
        if (Ovl11IsItemStatusNegative((s16) arg0[0]) == 0) {
            return 0;
        }
        if (gOvl11InteractionPhase != Ovl11ClassifyItemStatus((s16) arg0[0])) {
            return 0;
        }
        break;
    case 1:
        if (Ovl11IsItemStatusNegative((s16) arg0[0]) == 0) {
            return 0;
        }
    case 0:
        if (Ovl11GetItemType((s16) arg0[0]) == 0 && ((s16 *) arg0)[2] >= 2) {
            return 0;
        }
        break;
    }
    switch (Ovl11GetItemSlotLevel(arg1)) {
    case 2:
        if (Ovl11IsItemStatusNegative((s16) arg2[0]) == 0) {
            return 0;
        }
        if (gOvl11InteractionPhase != Ovl11ClassifyItemStatus((s16) arg2[0])) {
            return 0;
        }
        Ovl11ReorderSelectionTextures(arg0, arg1, arg2, arg3, 1);
        Ovl11SwapCopyRecords((Ovl11CopyRecord *) arg0, (Ovl11CopyRecord *) arg2);
        return 1;
    case 1:
        if (Ovl11IsItemStatusNegative((s16) arg2[0]) == 0) {
            return 0;
        }
    case 0:
        if (Ovl11GetItemType((s16) arg2[0]) != 0 || ((s16 *) arg2)[2] < 2) {
            Ovl11ReorderSelectionTextures(arg0, arg1, arg2, arg3, 1);
            Ovl11SwapCopyRecords((Ovl11CopyRecord *) arg0, (Ovl11CopyRecord *) arg2);
            return 1;
        }
        break;
    }
    return 0;
}

void Ovl11AccumulateItemStackCount(u16 *arg0, u16 *arg1);

s32 Ovl11MergeSlotRecords(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3) {
    s32 temp_v0;

    temp_v0 = Ovl11GetItemSlotLevel(arg3);
    if (temp_v0 >= 0) {
        if (temp_v0 >= 2) {
            if (temp_v0 != 2) {
                return 0;
            }
            goto block_5;
        }
        if (Ovl11GetItemType((s16) *arg0) == 2) {
block_5:
            if (Ovl11GetItemType((s16) *arg0) != 1) {
                Ovl11AccumulateItemStackCount(arg0, arg2);
                return 1;
            }
            goto block_7;
        }
        /* Duplicate return node #8. Try simplifying control flow for better match */
        return 0;
    }
block_7:
    return 0;
}

/* Fill each hole from the next occupied slot, preserving record order.
 * Explicit backedges preserve the original's index-based scan: expressing
 * either as a C loop lets this compiler introduce a six-byte pointer walker.
 * Keep the two index terms in the inner address (rather than caching 6*i). */
void Ovl11CompactItemSlots(s16 *slots, s16 count) {
    s32 slotIndex;
    s32 scanIndex;
    s32 remaining;
    Ovl11CopyRecord *record;
    Ovl11CopyRecord *empty;

    slotIndex = 0;
    if (count > 0) {
next_slot:
        record = (Ovl11CopyRecord *)(slots + slotIndex * 3);
        if (record->field_0 == 0) {
            scanIndex = 0;
            empty = record;
            remaining = count - slotIndex;
find_source:
            if (scanIndex < remaining) {
                record = (Ovl11CopyRecord *)(slots + (slotIndex * 3) + (scanIndex * 3));
                scanIndex += 1;
                if (record->field_0 == 0) {
                    goto find_source;
                }
                *empty = *record;
                memset(record, 0, 6);
            }
        }
        slotIndex += 1;
        if (slotIndex < count) {
            goto next_slot;
        }
    }
}

s16 *Ovl11GetItemSlotRecord(s32 column, s32 row) {
    Ovl11ItemState *base;
    Ovl11ItemState *selected;
    Ovl11ItemState *start;
    s32 offset;
    s32 index;
    s32 rowOffset;

    rowOffset = row * 8 - 9;
    index = rowOffset + column;
    if (gOvl11InteractionPhase == 0) {
        base = (Ovl11ItemState *)&D_80071A84;
        offset = column << 1;
        offset += column;
        offset <<= 1;
        start = base + 1;
    } else {
        base = &Ovl11Item_80071A8A;
        offset = column << 1;
        offset += column;
        offset <<= 1;
        start = base + 8;
    }
    do {
        selected = (Ovl11ItemState *)(offset + (u32)start);
    } while (0);
    offset = Ovl11GetItemSlotLevel(column);
    switch (offset) {
    case 0:
        return (s16 *)base;
    case 1:
        return (s16 *)selected;
    default:
        switch (gOvl11InteractionPhase) {
        case 0:
            return (s16 *)((Ovl11ItemState *)&gItemOverflowSlotBase + index);
        default:
            return (s16 *)(D_80071042 + index);
        case 2:
            return (s16 *)(D_80070D42_slots[0] + index);
        }
    }
}

s32 Ovl11GetItemSlotLevel(s32 slot) {
    if (slot == 0) {
        return 0;
    }
    if (slot < 9) {
        return 1;
    }
    return 2;
}

void Ovl11AccumulateItemStackCount(u16 *source, u16 *destination) {
    s32 sum;

    sum = destination[2] + source[2];
    destination[2] = sum;
    if ((s16)sum >= 100) {
        destination[2] = 99;
        source[2] = sum - 99;
    } else {
        Ovl11ClearItemStackEntry((s16 *)source);
    }
}

void Ovl11SwapCopyRecords(Ovl11CopyRecord *left, Ovl11CopyRecord *right) {
    Ovl11CopyRecord temp;

    temp = *left;
    *left = *right;
    *right = temp;
}

void Ovl11CopySlotRecord(Ovl11CopyRecord *source, Ovl11CopyRecord *destination) {
    *destination = *source;
}

/* Navigation and drawing share all 17 rows of the private geometry table.
 * The split halfword initialization of delta preserves the original word
 * scratch construction; page deltas are applied only in message state FF. */

void Ovl11UpdateMenuGridNavigation(s32 slotCount) {
    union { s32 value; u16 halves[2]; } delta;
    s32 previous;
    s32 buttons;
    s32 pageButtons;
    s32 direction;
    s32 state;

    delta.halves[0] = 0;
    delta.halves[1] = 0;
    if (IsMessageWindowStatusFinal() != 0) {
        buttons = gPad0Input->repeatButtons;
        if (buttons & 0xF000) {
            if (buttons & 0x1000) {
                direction = delta.value;
            } else if (buttons & 0x4000) {
                direction = 1;
            } else if (buttons & 0x8000) {
                if (gOvl11MenuGridCursorSlot == 9 || gOvl11MenuGridCursorSlot == 13) {
                    delta.value = slotCount / 8 - 1;
                }
                direction = 2;
            } else {
                if (gOvl11MenuGridCursorSlot == 12 || gOvl11MenuGridCursorSlot == 16) {
                    delta.value = 1;
                }
                direction = 3;
            }
            if (gOvl11MenuGridMessageState == 255) {
                gOvl11MenuGridCursorSlot = gOvl11MenuGridNavigation[gOvl11MenuGridCursorSlot].links[direction][gMenuGridLayoutIndex];
                PlaySoundEffect(5);
            }
        }
        if (gOvl11MenuGridMessageState == 255) {
            pageButtons = gPad0Input->repeatButtons;
            if (pageButtons & 5) {
                PlaySoundEffect(5);
                previous = delta.value - 1;
                delta.value = previous + slotCount / 8;
            } else if (pageButtons & 10) {
                PlaySoundEffect(5);
                delta.value++;
            }
        }
    }
    if (gOvl11MenuGridMessageState == 255) {
        gOvl11MenuGridPageIndex = (u16)gOvl11MenuGridPageIndex + delta.value - ((gOvl11MenuGridPageIndex + delta.value) / (slotCount / 8)) * (slotCount / 8);
    }
    if (gOvl11MenuGridSelectedSlot != 255) {
        state = Ovl11GetItemSlotLevel(gOvl11MenuGridSelectedSlot);
        if (state >= 0) {
            if (state < 2 || (state == 2 && gOvl11MenuGridPageIndex == gOvl11MenuGridSelectedPageIndex)) {
                Ovl11DrawSelectedSlotMarker(gActiveRenderContext->field_D8 + 0x54,
                    gOvl11MenuGridNavigation[gOvl11MenuGridSelectedSlot].x, gOvl11MenuGridNavigation[gOvl11MenuGridSelectedSlot].y);
            }
        }
    }
    Ovl11DrawSlotCursor(gActiveRenderContext->field_D8 + 0x50,
        gOvl11MenuGridNavigation[gOvl11MenuGridCursorSlot].x, gOvl11MenuGridNavigation[gOvl11MenuGridCursorSlot].y);
}

/* The two display arguments are retained but unused by this message path.
 * A4 records use the mapped item ID +1000, not the raw record ID. */
void Ovl11UpdateMenuGridMessage(s32 textDisplay, s32 panelDisplay, s32 slot, s32 page) {
    s16 *record;
    s32 message;

    if (gOvl11MenuGridMessageState != 255) {
        record = Ovl11GetItemSlotRecord(slot, page);
        if (record[0] == 0xA4) {
            message = Ovl11GetRecordItemId(record) + 1000;
        } else {
            message = record[0];
        }
        RequestMessageWindow(3, message);
        if (GetMessageWindowStatus() == 2) {
            gOvl11MenuGridMessageState = 255;
            CloseMessageWindow();
        }
    }
}

void Ovl11DrawMenuGridSlots(s32 *orderingTable, s32 firstSlot, s32 count, s16 *records) {
    s32 slot;

    for (slot = firstSlot; slot < firstSlot + count; slot++) {
        Ovl11DrawMenuGridSlot(orderingTable, slot, records);
        records += 3;
    }
}

void Ovl11DrawMenuGridSlot(s32 *orderingTable, s32 slot, s16 *record) {
    if (record[0] == 0) {
        Ovl11DrawEmptySlot((s32) (orderingTable + 1), gOvl11MenuGridNavigation[slot].x, gOvl11MenuGridNavigation[slot].y);
        return;
    }
    if (slot < 9) {
        Ovl11DrawSlotSprite(record[0], 0, orderingTable, gOvl11MenuGridNavigation[slot].x, gOvl11MenuGridNavigation[slot].y, 8);
    } else {
        Ovl11DrawTextureCell(orderingTable, (u16) record[0], gOvl11MenuGridNavigation[slot].x, gOvl11MenuGridNavigation[slot].y);
    }
    switch (Ovl11GetItemType(record[0])) {
    case 2:
        Ovl11DrawSlotQuantity(gActiveRenderContext->field_D8 + 0x58, gOvl11MenuGridNavigation[slot].x, gOvl11MenuGridNavigation[slot].y, record[2]);
        return;
    case 0:
        if (Ovl11GetItemSlotLevel(slot) == 2) {
            Ovl11DrawSlotQuantity(gActiveRenderContext->field_D8 + 0x58, gOvl11MenuGridNavigation[slot].x, gOvl11MenuGridNavigation[slot].y, record[2]);
        }
        /* fallthrough */
    case 1:
        return;
    }
}

/* Shared source is left on animation C after the two indicator draws. */
void Ovl11DrawMenuGridIndicators(s32 textDisplay) {
    s32 nextFrame;
    s32 wraps;

    SelectSpriteAnimation((ObjectState *) &gSharedSpriteSource, 0xD);
    SelectSpriteAnimationFrame((ObjectState *) &gSharedSpriteSource, gOvl11SharedSpriteFrame);
    DrawSpriteFrameRects(textDisplay, (s32) &gSharedSpriteSource, (s32) (*(u8 *) ((u8 *) &gSharedSpriteSource + 4)), (s32) (*(u8 *) ((u8 *) &gSharedSpriteSource + 5)), 0x7D, 0x90);
    SelectSpriteAnimation((ObjectState *) &gSharedSpriteSource, 0xC);
    SelectSpriteAnimationFrame((ObjectState *) &gSharedSpriteSource, gOvl11SharedSpriteFrame);
    DrawSpriteFrameRects(textDisplay, (s32) &gSharedSpriteSource, (s32) (*(u8 *) ((u8 *) &gSharedSpriteSource + 4)), (s32) (*(u8 *) ((u8 *) &gSharedSpriteSource + 5)), 0x115, 0x90);
    gOvl11SharedSpriteFrameTicks++;
    if (gOvl11SharedSpriteFrameTicks >= 9) {
        gOvl11SharedSpriteFrameTicks = 0;
        nextFrame = (s16) gOvl11SharedSpriteFrame + 1;
        wraps = nextFrame / 4;
        gOvl11SharedSpriteFrame = gOvl11SharedSpriteFrame - (wraps * 4 - 1);
    }
}

s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* One-based page / (slotCount/8), using shared FFFF-terminated digit scratch.
 * slotCount/8 must be nonzero; the screen driver supplies 64 slots. */
void Ovl11DrawMenuGridPageCount(s32 textDisplay, s32 slotCount) {
    s32 firstDisplay;
    s16 *end;

    end = gOvl11MenuGridPageText;
    end = func_8001A970(gOvl11MenuGridPageIndex + 1, end, 1);
    firstDisplay = textDisplay;
    *(u16 *)end = 0xFFFF;
    end = gOvl11MenuGridPageText;
    func_80017B3C(firstDisplay, (s32)end, 0x28, 0x6E);
    func_80017B3C(textDisplay, (s32)((u8 *)D_80051BF0 + gTextBankOffsets[0]), 0x35, 0x6E);
    *(u16 *)func_8001A970(slotCount / 8, gOvl11MenuGridPageText, 1) = 0xFFFF;
    func_80017B3C(textDisplay, (s32)gOvl11MenuGridPageText, 0x42, 0x6E);
}

void Ovl11DrawMenuGridPanels(u32 *panelOrderingTable, s32 textDisplay,
                           s32 topTextAddress, s32 bottomTextAddress) {
    DrawMenuPanel(panelOrderingTable, 1, 0x19, 0x10, 0x69, 0x4A);
    DrawMenuPanel(panelOrderingTable, 1, 0x79, 0x10, 0xAE, 0x4A);
    DrawMenuPanel(panelOrderingTable, 1, 0x19, 0x5A, 0x10E, 0x4A);
    Ovl11DrawMenuGridIndicators(textDisplay);
    func_80017B3C(textDisplay, topTextAddress, 0x1D, 0x14);
    func_80017B3C(textDisplay, bottomTextAddress, 0x1D, 0x5E);
}
