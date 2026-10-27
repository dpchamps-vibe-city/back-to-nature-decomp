/*
 * menu_grid_view — reconstructed translation unit.
 *
 * Navigation, selected-record messaging and layered rendering for the
 * interaction menu grid. Shares the private slot geometry/link table; range
 * rendering and animated indicators retain their internal helper calls.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_menu_grid_view.h"
#include "sound_commands.h"
#include "message_window.h"
#include "psyq/memory.h"
#include "ovl11_texture_cell_cache.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "packet_work.h"
#include "beveled_panel.h"
#include "psyq/libgpu.h"
#include "psyq/libsnd.h"
#include "psyq/libgs.h"
#include "psyq/libapi.h"
#include "ovl11_slot_sprite_cache.h"
#include "psyq/abs.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_slot_decorations.h"
#include "ovl11_cell_grid_lookup.h"
#include "sprite_source.h"
#include "sprite_render.h"

s32 ovl_11_func_800F8404(s32 arg0);

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
        buttons = *(s32 *)gPad0Input;
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
            pageButtons = *(s32 *)gPad0Input;
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
        state = ovl_11_func_800F8404(gOvl11MenuGridSelectedSlot);
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

s16 *ovl_11_func_800F82FC(s32 arg0, s32 arg1);

/* The two display arguments are retained but unused by this message path. */
void Ovl11UpdateMenuGridMessage(s32 textDisplay, s32 panelDisplay, s32 slot, s32 page) {
    s16 *record;
    s32 message;

    if (gOvl11MenuGridMessageState != 255) {
        record = ovl_11_func_800F82FC(slot, page);
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
        if (ovl_11_func_800F8404(slot) == 2) {
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
