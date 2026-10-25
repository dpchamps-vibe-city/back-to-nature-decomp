#ifndef OVL23_RECORD_VALUE_INCREMENTS_H
#define OVL23_RECORD_VALUE_INCREMENTS_H

#include "common.h"

/* Unchecked signed record index, 0x1D4 stride from work+0x8000.
 * Both fields are halfwords. At +0x19EA, a SIGNED value below 235 gains
 * 20 (low 16 bits), otherwise it becomes 255. Thus ordinary saturation
 * applies only to valid nonnegative byte-range inputs. No return value. */
void Ovl23RaiseRecordByteRangeValue(s16 recordIndex);

/* +0x19EC: unsigned value <=65034 gains 500, otherwise becomes 65535. */
void Ovl23RaiseRecordHalfwordRangeValue(s16 recordIndex);

#endif
