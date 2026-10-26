#ifndef OVL11_RECORD_ROW_VALUES_H
#define OVL11_RECORD_ROW_VALUES_H

#include "common.h"

/* Original ovl_11:800F508C. Visit all nineteen work lists (+DD8C pointers,
 * +DDD8 signed counts) and assign each 0x18-byte record's +2 halfword from
 * row 0..3 of gOvl11RecordRowValueTriples. No bounds/pointer checks.
 * Flag priority: 2000 selects the first signed value, else 0800 the second,
 * else 1000 the third, else zero. Only NONZERO results replace the old kind;
 * the stored halfword preserves the value's low bits. Flags are unchanged.
 * Nonpositive list counts perform no writes. No RNG or allocation effects. */
void Ovl11AssignPoolRowValues(s16 row);

#endif
