/*
 * Paired item-list actions, ovl_11:C8D9C-C8F9C. Each entry gates one
 * eight-record list against its selected record, compacts/exchanges it, then
 * publishes the selected triplet as the object's action payload. The two
 * protocols differ in their state and occupied-selection handling.
 */
#include "common.h"
#include "ovl11_item_list_exchange_actions.h"
#include "game_types.h"
#include "sound_commands.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_primary_object_state_change.h"
#include "ovl11_item_record_lists.h"
#include "ovl11_primary_object_requests.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_item_table_queries.h"

void Ovl11TryBeginStackListItemAction(Recon_ovl_11_func_800C7270_A0View *object, s32 unusedDirection) {
    u8 *record;
    s32 empty;
    u16 item;
    u32 i;
    u16 *selected;

    empty = 1;
    i = 0;
    record = (u8 *)object + 0x90;
    do {
        if (*(s16 *)record != 0) {
            empty = 0;
        }
        i += 1;
        record += 6;
    } while (i < 8U);
    item = object->unk84;
    if (empty == 0 || item != 0) {
        selected = &object->unk84;
        if ((s16)object->unk84 == 0 || Ovl11IsItemStatusNegative((s16)object->unk84) == 1) {
            object->unk6C &= 0xE7FFFFFF;
            Ovl11CompactItemStackList();
            if (Ovl11ExchangeItemStackList(0) != -1 && (s16)object->unk84 != 0) {
                Ovl11ChangeObjectStateDirect(object, 0x15, 0x47, 0xA);
                Ovl11CopyHalfwordTriplet(&object->unkF0, selected);
                object->unk6C |= 0x8000;
                PlaySoundEffect(7);
            }
        }
    }
}

void Ovl11TryBeginSecondaryListItemAction(Recon_ovl_11_func_800C7270_A0View *object, s32 unusedDirection) {
    s16 (*records)[3];
    s32 empty;
    u16 item;
    u32 i;
    s16 *selected;

    empty = 1;
    records = (s16 (*)[3])((u8 *)object + 0xC0);
    for (i = 0; i < 8U; i++) {
        if (records[i][0] != 0) {
            empty = 0;
        }
    }
    item = *(u16 *)((u8 *)object + 0x8A);
    if (empty == 0 || item != 0) {
        selected = (s16 *)((u8 *)object + 0x8A);
        if (*selected != 0) {
            if (Ovl11IsItemStatusNegative(*selected) == 1) {
                Ovl11CheckPrimaryObjectRequestSlot();
            }
        } else {
            object->unk6C &= 0xE7FFFFFF;
            Ovl11CompactSecondaryItemList();
            if (Ovl11ExchangeSecondaryItemList(0) != -1 && *(s16 *)((u8 *)object + 0x8A) != 0) {
                Ovl11ChangeObjectStateDirect(object, 0xA, *(u16 *)((u8 *)object + 0x38) + 0xB, -2);
                Ovl11CopyHalfwordTriplet(&object->unkF0, (u16 *)selected);
                object->unk6C |= 0x8000;
                PlaySoundEffect(7);
            }
        }
    }
}
