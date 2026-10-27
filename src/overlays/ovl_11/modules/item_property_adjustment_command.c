/*
 * item_property_adjustment_command — per-record adjustment/progress protocol.
 * Field widths, shared status-slot order and saturating writes are intentional.
 * Membership/boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_object_record_state.h"
#include "ovl11_tagged_item_codes.h"
#include "ovl11_record_adjustment.h"
#include "ovl11_item_property_adjustment_command.h"

s32 Ovl11ApplyItemPropertyAdjustmentCommand(s16 progressSlot, s16 recordSlot, s32 flagSlot) {
    Recon_ovl_11_func_8010CE80_A0View *record;
    s16 recordIndex;
    s16 adjustment;
    s16 i;
    s32 propertyCode;
    s32 index;
    s32 *slots;
    s32 *status;

    recordIndex = (s16) gOvl11ValueSlots[recordSlot];
    record = (Recon_ovl_11_func_8010CE80_A0View *)(gObjectRecordPool + recordIndex * 0xF8);
    record->unk34 |= 8;
    if (Ovl11StoreItemPropertyCode(0x1B, progressSlot, 0, 0) == 0 || (slots = &gOvl11ValueSlots[progressSlot], propertyCode = *slots) == 0) {
        index = recordIndex + 0xB;
        status = &gOvl11ValueSlots[index];
        if (*status != 2) {
            *status = 1;
            gOvl11ValueSlots[flagSlot] = 1;
            gOvl11ValueSlots[progressSlot] = 0xB;
        } else {
            gOvl11ValueSlots[progressSlot] = 0xC;
        }
        return 1;
    }
    if (propertyCode >= 4) {
        goto failure;
    }
    if (propertyCode <= 0) {
        goto failure;
    }
    if (gOvl11ValueSlots[flagSlot] != 0) {
        if (!(record->unkB8 & 0x10)) {
            record->unkB8 |= 0x10;
            adjustment = Ovl11LookupRecordAdjustment(recordIndex + 0x32, 0, *slots - 1);
        } else {
            adjustment = Ovl11LookupRecordAdjustment(recordIndex + 0x32, 2, *slots - 1);
        }
        if (*(s16 *)((u8 *)record + 0x16) < 0xFF - adjustment) {
            *(s16 *)((u8 *)record + 0x16) += adjustment;
        } else {
            *(s16 *)((u8 *)record + 0x16) = 0xFF;
        }
        slots = gOvl11ValueSlots;
        index = recordIndex + 0xB;
        slots[index] = 2;
        for (i = 0; i < 7; i++) {
            if (gOvl11ValueSlots[i + 0xB] != 2) {
                break;
            }
        }
        if (i != 7) {
            gOvl11ValueSlots[progressSlot] = 0xD;
        } else {
            gOvl11ValueSlots[progressSlot] = 0xE;
            record = (Recon_ovl_11_func_8010CE80_A0View *)gObjectRecordPool;
            for (i = 0; i < 7; i++, record = (Recon_ovl_11_func_8010CE80_A0View *)((u8 *)record + 0xF8)) {
                if (*(s16 *)((u8 *)record + 0x16) < 0xF5) {
                    *(s16 *)((u8 *)record + 0x16) += 0xA;
                } else {
                    *(s16 *)((u8 *)record + 0x16) = 0xFF;
                }
            }
        }
        return 1;
    }
failure:
    Ovl11SetObjectRecordMode(record, 6);
    return 0;
}
