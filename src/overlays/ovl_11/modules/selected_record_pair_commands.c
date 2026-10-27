/*
 * Four complementary commands over the four selected record pairs. E9D28's
 * opcode 12 groups restore, test, required-text editing and marked cleanup.
 * The text editor targets the first record, but the lower-bank marker test
 * targets the second: these are deliberately different pair components.
 * Contracts and unchecked-index requirements are in the public header.
 */
#include "common.h"
#include "ovl11_selected_record_pair_commands.h"
#include "game_types.h"
#include "ovl11_record_pair_slots.h"
#include "text_entry.h"

void Ovl11RestoreSelectedRecordPairs(void) {
    Ovl11RestorePairedRecords(1);
    Ovl11RestorePairedRecords(0);
}

s32 Ovl11IsSelectedRecordPairMarked(s16 selector) {
    if ((selector == 1 && ((Ovl11Status99C8View *)D_8006C838)->field_44D0 == 0) ||
        (selector == 3 && ((Ovl11Status99C8View *)D_8006C838)->field_44D2 == 0)) {
        return 0;
    }
    if (!(selector & 2)) {
        if (((struct Ovl11PoolResetView *)D_8006C838)->records_81BC[
                ((struct Ovl11PoolResetView *)D_8006C838)->pairs_99C8[selector][1]].flags & 0x200) {
            return 1;
        }
    } else {
        if (((struct Ovl11PoolResetView *)D_8006C838)->records_7AB4[
                ((struct Ovl11PoolResetView *)D_8006C838)->pairs_99C8[selector][0]].type == 0x10A) {
            return 1;
        }
    }
    return 0;
}

s32 Ovl11EditSelectedRecordText(s16 selector) {
    Ovl11StatusTextRecord *destination;
    s32 result;
    s32 textEntryStatus;

    if (selector < 0) {
        destination = &gOvl11StatusTextScratch;
    } else if (selector < 2) {
        destination = (Ovl11StatusTextRecord *)((u8 *)&D_800749F8
            + ((Ovl11Status99C8View *)D_8006C838)->field_99C8[selector * 2] * 0xB8);
    } else if (selector < 4) {
        destination = (Ovl11StatusTextRecord *)((u8 *)D_8006C838 + 0x7AB8
            + ((Ovl11Status99C8View *)D_8006C838)->field_99C8[selector * 2] * 0xB4);
    } else {
        destination = &gOvl11StatusTextScratch;
    }

    result = 0;
    textEntryStatus = UpdateTextEntry();
    if (textEntryStatus != 0) {
        if (textEntryStatus > 0) {
            if (textEntryStatus == 1) {
                *destination = gOvl11StatusTextScratch;
                result = 1;
            }
        }
    } else {
        BeginRequiredTextEntry((u16 *)&gOvl11StatusTextScratch);
    }
    return result;
}

void Ovl11ClearMarkedRecordPairs(void) {
    s32 pairIndex;
    s32 *flags;
    s32 index;
    s32 offset;
    s16 firstRecordIndex;
    s16 *pair;
    s32 pairOffset;
    u8 *work;

    for (pairIndex = 0; pairIndex < 4; pairIndex++) {
        work = (u8 *)D_8006C838;
        pairOffset = 0x99C8;
        if (Ovl11IsSelectedRecordPairMarked((s16)pairIndex) == 1) {
            if ((u32)pairIndex < 2U) {
                index = ((Ovl11ClearPairWorkView *)D_80074838)->pairs[pairIndex][1];
                offset = index * -0xB8;
                flags = (s32 *)((u8 *)((Ovl11ClearFlagWorkView *)D_80074838)->records - offset);
                index = ~0x200;
                *flags &= index;
                pair = (s16 *)(work + pairIndex * 4 + pairOffset);
                firstRecordIndex = *pair;
                index = firstRecordIndex;
                offset = index * -0xB8;
                flags = (s32 *)((u8 *)((Ovl11ClearFlagWorkView *)D_80074838)->records - offset);
                *flags &= 0xFDFFFFFF;
            }
            Ovl11ClearRecordPairSlot((Ovl11RecordPairSlot *)
                ((struct Ovl11PoolResetView *)D_8006C838)->pairs_99C8[pairIndex]);
        }
    }
    Ovl11ResyncRecordRowPlacements(1);
    Ovl11ResyncRecordRowPlacements(0);
}
