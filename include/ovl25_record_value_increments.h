#ifndef OVL25_RECORD_VALUE_INCREMENTS_H
#define OVL25_RECORD_VALUE_INCREMENTS_H

#include "common.h"

/* recordIndex is sign-extended and unchecked. Both values occupy halfwords
 * in the 0x1D4-stride work-record array based at work+0x8000.
 * The +0x19EA value uses a SIGNED comparison: below 235 add 20 (low 16
 * bits), otherwise store 255. Only valid nonnegative byte-range inputs
 * behave as an ordinary saturating addition. */
void Ovl25RaiseRecordByteRangeValue(s16 recordIndex);

/* +0x19EC: unsigned value <=65034 gains 500, otherwise becomes 65535. */
void Ovl25RaiseRecordHalfwordRangeValue(s16 recordIndex);

#endif
