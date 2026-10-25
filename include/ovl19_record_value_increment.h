#ifndef OVL19_RECORD_VALUE_INCREMENT_H
#define OVL19_RECORD_VALUE_INCREMENT_H

#include "common.h"

/* ovl_19:800BBD14. Unchecked signed record index; stride 0x1D4 from
 * shared work+0x8000. At +0x19EA a SIGNED halfword below 235 gains 20
 * (low 16 bits); otherwise it becomes 255. Ordinary saturation applies
 * only to valid nonnegative byte-range inputs. No return value.
 * This overlay contains no corresponding +0x19EC halfword-range raiser. */
void Ovl19RaiseRecordByteRangeValue(s16 recordIndex);

#endif
