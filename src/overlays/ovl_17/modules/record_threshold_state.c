/*
 * Threshold-roll classification of six 0x50-byte record views. The selected
 * comparison record chooses the primary roll's orientation; a nearby-value
 * record enables the secondary roll. This service consumes shared RNG state.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl17_record_threshold_state.h"
#include "random.h"

s32 Ovl17HasRecordInValueWindow(s16 recordIndex);
s32 Ovl17RollSecondaryThreshold(s16 threshold);
s32 Ovl17RollPrimaryThreshold(s16 threshold);
s32 Ovl17RollComplementThreshold(s16 threshold);
s32 Ovl17RollFraction127(s32 numerator);

s32 Ovl17EvaluateRecordThresholdState(s16 recordIndex) {
    s32 result;
    s32 primaryState;
    s32 (*rollPrimary)(s16);
    u8 *work;
    u8 *recordBase;
    u8 *values;
    u8 *record;

    work = gOvl17RecordWorkBase;
    values = work + 0x38;
    if (*(s32 *)(values + *(s16 *)(work + 0x2BC) * 0x50) >= (*(s16 *)(work + 0x2D2) << 12)) {
        rollPrimary = Ovl17RollPrimaryThreshold;
    } else {
        rollPrimary = Ovl17RollComplementThreshold;
    }
    recordBase = gOvl17RecordWorkBase;
    record = recordBase + recordIndex * 0x50;
    primaryState = (rollPrimary(*(s16 *)(record + 0x42)) != 0) ? 1 : 2;
    result = primaryState;
    if (Ovl17HasRecordInValueWindow(recordIndex) != 0) {
        if (Ovl17RollSecondaryThreshold(*(s16 *)(record + 0x44)) != 0) {
            result = primaryState - 1;
        }
    }
    return result;
}

s32 Ovl17RollPrimaryThreshold(s16 threshold) {
    return Ovl17RollFraction127(threshold + 1);
}

s32 Ovl17RollComplementThreshold(s16 threshold) {
    return Ovl17RollFraction127(0x7F - threshold);
}

/* Separate original entry for the second field, despite the identical body. */
s32 Ovl17RollSecondaryThreshold(s16 threshold) {
    return Ovl17RollFraction127(threshold + 1);
}

s32 Ovl17RollFraction127(s32 numerator) {
    u16 percent = (numerator * 100) / 127;

    return Rand(0x64) < percent;
}

s32 Ovl17HasRecordInValueWindow(s16 recordIndex) {
    u8 *work;
    s32 *values;
    s32 *selectedValue;
    s32 i;
    s32 difference;

    i = 0;
    work = gOvl17RecordWorkBase;
    values = (s32 *)(work + 0x38);
    selectedValue = (s32 *)((u8 *)values + recordIndex * 0x50);
    for (; i < 6; i++) {
        if (i == recordIndex) {
            continue;
        }
        difference = *(s32 *)((u8 *)values + i * 0x50) - *selectedValue;
        if (difference < 0) {
            continue;
        }
        if (difference > 0x59FFF) {
            continue;
        }
        return 1;
    }
    return 0;
}
