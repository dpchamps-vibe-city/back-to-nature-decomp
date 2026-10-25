/*
 * Table-driven random decisions for overlay 23's record controller.
 * The two private probability tables serve its weighted result selection
 * and subsequent threshold-bucket trials. No record state is modified.
 * Membership and conservative outer boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "random.h"
#include "ovl23_record_random_selection.h"

s32 Ovl23TestRecordRandomThreshold(s16 trialRow, s16 recordIndex) {
    s16 difference;
    s16 threshold;
    s32 bucket;
    u8 *state;

    state = Ovl23_D_800BF87C_draw;
    difference = (s16)(*(u16 *)(state + recordIndex * 0x50 + 0x30) -
                       *(u16 *)(state + recordIndex * 0x50 + 0x32));
    if (*(s16 *)(state + 0x3A4) < difference) {
        bucket = 0;
    } else if (*(s16 *)(state + 0x3A6) < difference) {
        bucket = 1;
    } else if (*(s16 *)(state + 0x3A8) < difference) {
        bucket = 2;
    } else if (*(s16 *)(state + 0x3AA) < difference) {
        bucket = 3;
    } else {
        bucket = 4;
    }
    threshold = gOvl23RecordRandomTrialThresholds[trialRow * 5 + bucket];
    return threshold > (s32)Rand(0x64);
}

s32 Ovl23ChooseRecordWeightedResult(s16 recordIndex, s16 weightSet) {
    s32 row;
    s16 cumulativeWeight;
    s16 draw;
    s32 i;
    s32 weight;
    u8 *state;
    s32 recordPositionQ12;
    s32 actorPositionQ12;
    s32 distance;
    s32 recordPosition;
    s32 distanceThreshold;
    u8 *categoryState;

    cumulativeWeight = 0;
    draw = (s16)Rand(0x64);
    if (weightSet == 0) {
        categoryState = Ovl23_D_800BF87C_draw;
        row = (u16)(((Ovl23D87CView50 *)categoryState)->unk0[recordIndex].unk28 - 1) < 2;
    } else {
        state = Ovl23_D_800BF87C_draw;
        recordPositionQ12 = ((Ovl23D87CView210 *)state)->unk210[recordIndex].unk0;
        /* Add the fractional mask before shifting negative Q12 values:
         * these conversions truncate toward zero, not toward minus infinity. */
        if (recordPositionQ12 < 0) {
            recordPositionQ12 += 0xFFF;
        }
        recordPosition = recordPositionQ12 >> 12;
        distanceThreshold = recordIndex * 0x50;
        actorPositionQ12 = ((Ovl23D87CView50 *)state)->unk0[recordIndex].unk38;
        if (actorPositionQ12 < 0) {
            actorPositionQ12 += 0xFFF;
        }
        distance = recordPosition - (actorPositionQ12 >> 12);
        distanceThreshold = *(s16 *)((u8 *)state + distanceThreshold + 0x2E);
        if (distance < 0) {
            distance = -distance;
        }
        row = (distance - distanceThreshold) < 0x65;
    }
    for (i = 0; i < 3; i++) {
        weight = *(u16 *)((u8 *)gOvl23RecordRandomWeights + row * 6 + weightSet * 12 + i * 2);
        cumulativeWeight = (s16)(cumulativeWeight + weight);
        if (draw < cumulativeWeight) {
            break;
        }
    }
    if (i == 0) {
        return 3;
    }
    if (i == 1) {
        return 2;
    }
    if (i == 2) {
        return 1;
    }
    return -1;
}
