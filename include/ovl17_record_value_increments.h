#ifndef OVL17_RECORD_VALUE_INCREMENTS_H
#define OVL17_RECORD_VALUE_INCREMENTS_H

#include "common.h"

/* Unchecked signed record index; stride 0x1D4 from shared work+0x8000.
 * Both fields are halfwords. At +0x19EA a SIGNED value below 235 gains
 * 20 (low 16 bits); otherwise it becomes 255. Ordinary saturation thus
 * applies only to valid nonnegative byte-range inputs. No return value. */
void Ovl17RaiseRecordByteRangeValue(s16 recordIndex);

/* +0x19EC: unsigned value <=65034 gains 500, otherwise becomes 65535. */
void Ovl17RaiseRecordHalfwordRangeValue(s16 recordIndex);

#endif
