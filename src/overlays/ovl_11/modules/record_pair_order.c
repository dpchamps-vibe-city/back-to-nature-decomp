/*
 * Table-driven order classification and a four-valued record-code selector.
 * The field offsets are established, but their gameplay coordinate meanings
 * are not. Keep them neutral. Interface: ovl11_record_pair_order.h.
 * Membership/boundary evidence: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "psyq/abs.h"
#include "ovl11_record_pair_order.h"

u16 Ovl11ClassifyRecordPairOrder(Ovl11RankPair *record) {
    /* Original calls pass a static chain; keep this a nested definition. */
    s32 Ovl11CompareRecordOrderRank(s32 first, s32 second) {
        if (first == second) {
            return 0;
        }
        if (first < second) {
            return 1;
        }
        return 2;
    }
    s32 firstRank;
    s32 secondRank;

    firstRank = Ovl11CompareRecordOrderRank(record->unk38, record->unk58);
    secondRank = Ovl11CompareRecordOrderRank(record->unk40, record->unk60);
    return gOvl11RecordPairOrderCodes[firstRank * 3 + secondRank];
}

void Ovl11SelectRecordPairCode(Recon_ovl_11_func_800D2594_A0View *record) {
    s32 preferCodes1Or3;
    s32 secondDelta;
    s32 orderClass;

    preferCodes1Or3 = abs((*(s32 *)((u8 *)record + 0x38)) - (*(s32 *)((u8 *)record + 0x58)));
    secondDelta = abs(record->unk3C - (*(s32 *)((u8 *)record + 0x5C)));
    preferCodes1Or3 = secondDelta < preferCodes1Or3;
    orderClass = Ovl11ClassifyRecordPairOrder((Ovl11RankPair *)record);
    switch (orderClass) {
    case 1:
    case 5:
        (*(s16 *)((u8 *)record + 0x22)) = preferCodes1Or3 ? 3 : 2;
        break;
    case 2:
    case 6:
        (*(s16 *)((u8 *)record + 0x22)) = preferCodes1Or3 ? 3 : 0;
        break;
    case 3:
    case 7:
        (*(s16 *)((u8 *)record + 0x22)) = preferCodes1Or3 ? 1 : 0;
        break;
    case 4:
    case 8:
        (*(s16 *)((u8 *)record + 0x22)) = preferCodes1Or3 ? 1 : 2;
        break;
    }
}
