/*
 * record_decision_tests — reconstructed translation unit.
 *
 * Decision-table inputs: compare signed per-record fields with thresholds and
 * roll the selected signed threshold against a 0..126 RNG sample. These
 * three helpers share one decision-hub client; indices and thresholds are
 * unchecked.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl17_record_decision_tests.h"
#include "random.h"

s32 Ovl17IsRecordAboveDecisionThreshold(s16 recordIndex, s16 threshold) {
    u8 *base;
    s32 scaled;

    base = gOvl17RecordWorkBase;
    scaled = recordIndex * 0x50;
    return (s16)threshold < *(s16 *)(base + scaled + 0x2A);
}

s32 Ovl17IsRecordDecisionReady(s16 recordIndex) {
    u8 *base;
    s32 scaled;

    base = gOvl17RecordWorkBase;
    scaled = recordIndex * 0x50;
    return *(s16 *)(base + scaled + 0x36) >= 0x51;
}

s32 Ovl17RollAboveDecisionThreshold(s16 threshold) {
    s32 sample;

    sample = Rand(0x7F);
    return threshold < sample;
}
