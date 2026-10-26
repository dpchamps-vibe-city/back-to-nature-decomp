#ifndef OVL17_RECORD_THRESHOLD_STATE_H
#define OVL17_RECORD_THRESHOLD_STATE_H

#include "common.h"

/*
 * Classify recordIndex (unchecked, 0..5) in gOvl17RecordWorkBase's 0x50-byte
 * record views. Returns 1 for a successful primary roll, otherwise 2; subtracts
 * one when another record's +0x38 value is within [0, 0x59FFF] above this one's
 * AND the secondary roll succeeds. Equal values count; the record itself does
 * not. No gameplay or physical unit is assigned to that value.
 *
 * The primary numerator is +0x42 + 1 or 127 - +0x42, selected by comparing
 * record[work.s16@+0x2BC].s32@+0x38 with work.s16@+0x2D2 << 12. The secondary
 * uses +0x44 + 1. Each roll tests Rand(100) < (u16)(numerator * 100 / 127): no
 * clamp, signed division, then halfword truncation. Intended numerators 0..127
 * give a percentage threshold, not a direct Rand(127) test.
 *
 * Reads record/control storage without modifying it. Always consumes one RNG
 * call; consumes a second only when the value-window query succeeds.
 */
s32 Ovl17EvaluateRecordThresholdState(s16 recordIndex);

#endif
