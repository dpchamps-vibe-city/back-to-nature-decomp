#ifndef OVL17_RECORD_DECISION_TESTS_H
#define OVL17_RECORD_DECISION_TESTS_H

#include "common.h"

/* Inputs to the six-record decision table (recordIndex: unchecked 0..5).
 * These queries read the current work area without modifying the record.
 * The signed +2A value selects a threshold band; +36 >= 81 selects the ready
 * column. Their broader gameplay meanings and units are not established. */
s32 Ovl17IsRecordAboveDecisionThreshold(s16 recordIndex, s16 threshold);
s32 Ovl17IsRecordDecisionReady(s16 recordIndex);

/* Consume one shared RNG sample in 0..126 and return sample > threshold.
 * No clamping: negative thresholds always pass, thresholds >= 126 never do.
 * This is a raw threshold, NOT a percentage or the rounded /127 roll used
 * by the separate record-threshold-state classifier. */
s32 Ovl17RollAboveDecisionThreshold(s16 threshold);

#endif
