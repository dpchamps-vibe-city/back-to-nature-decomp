/*
 * Two-sided item-transfer menu (ovl_11:800FAAAC..800FB5FC).
 *
 * The menu displays two special slots and up to eight regular slots per
 * side. It can inspect an item, move it into an empty slot, swap unlike
 * items or merge equal stackable items. A transfer returns 0 on success,
 * 1 on rejection; opening/updating use the legacy interface in the header.
 * Membership and original-boundary evidence: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_item_transfer_menu.h"
#include "sound_commands.h"
#include "message_window.h"
#include "screen_primitives.h"
#include "integer_power.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "beveled_panel.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/libgpu.h"
#include "psyq/libsnd.h"
#include "psyq/memory.h"
#include "psyq/libgs.h"
#include "psyq/libapi.h"
#include "ovl11_slot_sprite_cache.h"
#include "psyq/abs.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_slot_decorations.h"
#include "ovl11_held_item_flags.h"
#include "ovl11_item_stack_clear.h"
#include "ovl11_menu_exit_flags.h"
#include "ui_sprites.h"

u32 func_80017A64(void);
void func_80017A48(u32 style);
void func_80017A38(s16 x, s16 y);
void SetVal8005E2BC(s32 value);
void func_80017B3C(s32 ot, s32 text, s32 x, s32 y);

/* Internal interfaces retain their original external linkage and word-sized
 * pointer arguments where present. Slot helpers are not a general inventory API. */
void Ovl11DrawAndProcessItemTransferMenu(void);
void Ovl11ShowItemTransferEntryMessage(s16 *item);
s32 Ovl11TryTransferMenuItems(s32 sourceSlot, s32 destinationSlot);
s32 Ovl11TryMoveItemTransferEntry(s32 source, s32 sourceSlot, s32 destination, s32 destinationSlot);
s32 Ovl11TrySwapItemTransferEntries(Ovl11ItemState *source, s32 sourceSlot,
                                   Ovl11ItemState *destination, s32 destinationSlot);
s32 Ovl11TryMergeItemTransferStacks(Ovl11ItemState *source, s32 sourceSlot,
                                  s32 destination, s32 destinationSlot);
s32 Ovl11IsItemTransferSpecialSlot(s32 slot);
void Ovl11MergeItemTransferStacks(u16 *source, u16 *destination);
void Ovl11SwapItemTransferEntries(Ovl11ItemState *first, Ovl11ItemState *second);
char *Ovl11GetItemTransferEntry(s32 slot);
void Ovl11DrawItemTransferEntry(s32 ot, s32 slot, s16 *item);

void Ovl11ResetItemTransferMenu(void) {
    gOvl11ItemTransferMenuState = 0;
    gOvl11ItemTransferCursor = 0;
    gOvl11ItemTransferSavedCursor = 0;
    gOvl11ItemTransferSelectedSlot = 0xFF;
}

s32 Ovl11OpenItemTransferMenu(void) {
    if (gOvl11ItemTransferMenuState == 0) {
        Ovl11ResetItemTransferMenu();
        gOvl11ItemTransferMenuState = 1;
        PlaySoundEffect(3);
    } else {
        return gOvl11ItemTransferMenuState;
    }
    /* The original open path has no defined C return value. Callers discard
     * it; adding a return would change the original instruction stream. */
}

void Ovl11UpdateItemTransferMenu(void) {
    u32 previousStyle;
    u32 *workFlags;

    if (gOvl11ItemTransferMenuState != 0) {
        DrawScreenTileRect((u32 *)(gActiveRenderContext->field_D8 + 0x68), 0, 0,
            (s16)HWD0, (s16)VWD0, 0, 1);
        if (gOvl11ItemTransferMenuState == 3) {
            if (GetMessageWindowStatus() == 2) {
                ResetMessageWindow();
                gOvl11ItemTransferMenuState = 0;
                Ovl11PostMenuExitFlags();
                workFlags = (u32 *)D_8006C838;
                workFlags[3] |= 0x40000;
                workFlags[0x5234 / 4] |= 0x2000;
            }
        } else {
            previousStyle = func_80017A64();
            func_80017A48(3);
            Ovl11DrawAndProcessItemTransferMenu();
            func_80017A48(previousStyle);
            Ovl11SyncHeldItemFlags((Ovl11C7010Object *)&gPrimaryObjectBase);
        }
    }
}

void Ovl11DrawAndProcessItemTransferMenu(void) {
    s32 i;
    s32 textOt;
    s32 itemOt;
    s32 navigationGroup;
    s32 state;
    u32 buttons;
    u32 movement;
    s32 failed;

    textOt = gActiveRenderContext->field_D8 + 0x60;
    itemOt = gActiveRenderContext->field_D8 + 0x5C;
    func_80017A38(0, 2);
    for (i = 0; i < 4U; i++) {
        DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x64), 1,
            gOvl11ItemTransferPanels[i][0], gOvl11ItemTransferPanels[i][1],
            gOvl11ItemTransferPanels[i][2], gOvl11ItemTransferPanels[i][3]);
    }
    func_80017B3C(textOt, (s32)(D_80051B7C + gTextBankOffsets[0]), 0x1D, 0x14);
    func_80017B3C(textOt, (s32)(D_80051B7C + 0xC + gTextBankOffsets[0]), 0x1D, 0x5E);
    Ovl11DrawItemTransferEntry(itemOt, 0, (s16 *)((u8 *)D_8006C838 + 0x524C));
    Ovl11DrawItemTransferEntry(itemOt, 9, (s16 *)((u8 *)D_8006C838 + 0x5252));
    for (i = 0; i < pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1); i++) {
        Ovl11DrawItemTransferEntry(itemOt, i + 1, (s16 *)((u8 *)D_8006C838 + 0x5258 + i * 6));
        Ovl11DrawItemTransferEntry(itemOt, i + 10, (s16 *)((u8 *)D_8006C838 + 0x5288 + i * 6));
    }
    navigationGroup = 4;
    if (gOvl11ItemTransferSelectedSlot != 0xFF) {
        Ovl11DrawSelectedSlotMarker(gActiveRenderContext->field_D8 + 0x54,
            gOvl11ItemTransferLayout[gOvl11ItemTransferSelectedSlot][0],
            gOvl11ItemTransferLayout[gOvl11ItemTransferSelectedSlot][1]);
    } else {
        navigationGroup = 0;
    }
    state = gOvl11ItemTransferMenuState;
    if (state != 1) {
        Ovl11ShowItemTransferEntryMessage((s16 *)Ovl11GetItemTransferEntry(gOvl11ItemTransferCursor));
        return;
    }
    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x3AC);
    if (IsMessageWindowStatusFinal()) {
        buttons = ((u32 *)gPad0Input)[2];
        if (buttons & 0x10) {
            if (gOvl11ItemTransferMenuState == state) {
                if (*(s16 *)Ovl11GetItemTransferEntry(gOvl11ItemTransferCursor)) {
                    PlaySoundEffect(0);
                    CloseMessageWindow();
                    gOvl11ItemTransferMenuState = 2;
                } else {
                    PlaySoundEffect(2);
                }
            }
        } else if (buttons & 0x20) {
            if (gOvl11ItemTransferSelectedSlot == 0xFF) {
                CloseMessageWindow();
                gOvl11ItemTransferMenuState = 3;
                PlaySoundEffect(3);
                return;
            }
            PlaySoundEffect(1);
            gOvl11ItemTransferSelectedSlot = 0xFF;
        } else if (buttons & 0x40) {
            if (gOvl11ItemTransferSelectedSlot == 0xFF) {
                PlaySoundEffect(0);
                gOvl11ItemTransferSelectedSlot = gOvl11ItemTransferCursor;
            } else {
                gOvl11ItemTransferSavedCursor = gOvl11ItemTransferCursor;
                /* Selecting the source twice targets that side's special
                 * slot. Restore the visible cursor after the transfer. */
                if (gOvl11ItemTransferSelectedSlot == gOvl11ItemTransferCursor) {
                    gOvl11ItemTransferCursor = gOvl11ItemTransferSelectedSlot < 9 ? 0 : 9;
                }
                failed = Ovl11TryTransferMenuItems(gOvl11ItemTransferSelectedSlot, gOvl11ItemTransferCursor);
                gOvl11ItemTransferCursor = gOvl11ItemTransferSavedCursor;
                if (!failed) {
                    PlaySoundEffect(0);
                    gOvl11ItemTransferSelectedSlot = 0xFF;
                } else {
                    PlaySoundEffect(2);
                }
            }
        } else if (buttons & 0x800) {
            CloseMessageWindow();
            PlaySoundEffect(3);
            gOvl11ItemTransferMenuState = 3;
        } else {
            movement = ((u32 *)gPad0Input)[0] & 0xF000;
            if (movement) {
                switch (movement) {
                case 0x1000:
                    break;
                case 0x4000:
                    navigationGroup++;
                    break;
                case 0x8000:
                    navigationGroup += 2;
                    break;
                case 0x2000:
                    navigationGroup += 3;
                    break;
                }
                gOvl11ItemTransferCursor = gOvl11ItemTransferNavigation[gOvl11ItemTransferCursor].destinations[
                    ((struct Ovl11ItemGridState *)D_8006C838)->exponent + navigationGroup * 3];
                PlaySoundEffect(5);
            }
        }
    }
    DrawUiSprite0BFrame1(gActiveRenderContext->field_D8 + 0x50,
        (s16)(gOvl11ItemTransferLayout[gOvl11ItemTransferCursor][0] + 12),
        (s16)(gOvl11ItemTransferLayout[gOvl11ItemTransferCursor][1] + 24));
}

void Ovl11ShowItemTransferEntryMessage(s16 *item) {
    s32 message = item[0];

    if (message == 0xA4) {
        message = Ovl11GetRecordItemId(item) + 0x3E8;
    }
    RequestMessageWindow(3, message);
    if (GetMessageWindowStatus() == 2) {
        gOvl11ItemTransferMenuState = 1;
        CloseMessageWindow();
    }
}

s32 Ovl11TryTransferMenuItems(s32 sourceSlot, s32 destinationSlot) {
    s16 *source;
    s16 *destination;

    if (sourceSlot == destinationSlot) {
        return 1;
    }
    source = (s16 *)Ovl11GetItemTransferEntry(sourceSlot);
    destination = (s16 *)Ovl11GetItemTransferEntry(destinationSlot);
    if (source[0] == 0) {
        if (destination[0] == 0) {
            return 0;
        }
        goto moveFromDestination;
    }
    if (destination[0] == 0) {
        return Ovl11TryMoveItemTransferEntry((s32)source, sourceSlot, (s32)destination, destinationSlot);
    }
moveFromDestination:
    if (source[0] == 0) {
        return Ovl11TryMoveItemTransferEntry((s32)destination, destinationSlot, (s32)source, sourceSlot);
    }
    if (source[0] == destination[0]) {
        return Ovl11TryMergeItemTransferStacks((Ovl11ItemState *)source, sourceSlot,
                                              (s32)destination, destinationSlot);
    }
    return Ovl11TrySwapItemTransferEntries((Ovl11ItemState *)source, sourceSlot,
                                           (Ovl11ItemState *)destination, destinationSlot);
}

s32 Ovl11TryMoveItemTransferEntry(s32 source, s32 sourceSlot, s32 destination, s32 destinationSlot) {
    s32 special;

    special = Ovl11IsItemTransferSpecialSlot(destinationSlot);
    /* The nested case label is intentional: ordinary slots require the
     * item predicate, while the two special slots bypass that test. */
    switch (special) {
    case 0:
        if (Ovl11IsItemStatusNegative(((Ovl11ItemState *)source)->id) != 0) {
        case 1:
            Ovl11SwapItemTransferEntries((Ovl11ItemState *)source, (Ovl11ItemState *)destination);
            return 0;
        }
    default:
        return 1;
    }
}

s32 Ovl11TrySwapItemTransferEntries(Ovl11ItemState *source, s32 sourceSlot,
                                   Ovl11ItemState *destination, s32 destinationSlot) {
    switch (Ovl11IsItemTransferSpecialSlot(destinationSlot)) {
    case 0:
        if (Ovl11IsItemStatusNegative(source->id) == 0) {
            return 1;
        }
        /* fall through */
    case 1:
        if (Ovl11GetItemType(source->id) != 0 || source->unk4 < 2) {
            break;
        }
        return 1;
    default:
        break;
    }

    switch (Ovl11IsItemTransferSpecialSlot(sourceSlot)) {
    case 0:
        if (Ovl11IsItemStatusNegative(destination->id) == 0) {
            return 1;
        }
        /* fall through */
    case 1:
        if (Ovl11GetItemType(destination->id) != 0 || destination->unk4 < 2) {
            Ovl11SwapItemTransferEntries(source, destination);
            return 0;
        }
        return 1;
    default:
        return 1;
    }
}

s32 Ovl11TryMergeItemTransferStacks(Ovl11ItemState *source, s32 sourceSlot,
                                  s32 destination, s32 destinationSlot) {
    s32 itemClass;

    itemClass = Ovl11GetItemType(source->id);
    if (itemClass == 2) {
        Ovl11MergeItemTransferStacks((u16 *)source, (u16 *)destination);
        return 0;
    } else {
        return 1;
    }
}

s32 Ovl11IsItemTransferSpecialSlot(s32 slot) {
    if (slot == 0 || slot == 9) {
        return 1;
    }
    return 0;
}

/* Counts are read unsigned, then the sum is compared as a signed halfword.
 * Normal UI inputs are small counts; preserve this behavior, including the
 * absence of validation. Keep excess in the source when the cap is exceeded. */
void Ovl11MergeItemTransferStacks(u16 *source, u16 *destination) {
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

void Ovl11SwapItemTransferEntries(Ovl11ItemState *first, Ovl11ItemState *second) {
    Ovl11ItemState temporary;

    temporary = *first;
    *first = *second;
    *second = temporary;
}

char *Ovl11GetItemTransferEntry(s32 slot) {
    if (slot == 0) {
        return (char *)&D_80071A84;
    }
    if (slot == 9) {
        return (char *)&Ovl11Item_80071A8A;
    }
    if (slot < 9) {
        return (char *)&Ovl11Item_80071A8A + (slot * 6);
    }
    return (char *)&D_80071A84 + (slot * 6);
}

void Ovl11DrawItemTransferEntry(s32 ot, s32 slot, s16 *item) {
    s16 id;

    id = item[0];
    if (id == 0) {
        Ovl11DrawEmptySlot(ot, gOvl11ItemTransferLayout[slot][0], gOvl11ItemTransferLayout[slot][1]);
        return;
    }
    Ovl11DrawSlotSprite(id, 0, (s32 *)ot,
        gOvl11ItemTransferLayout[slot][0], gOvl11ItemTransferLayout[slot][1], 8);
    if (Ovl11GetItemType(item[0]) == 2) {
        Ovl11DrawSlotQuantity(gActiveRenderContext->field_D8 + 0x58,
            gOvl11ItemTransferLayout[slot][0], gOvl11ItemTransferLayout[slot][1], item[2]);
    }
}
