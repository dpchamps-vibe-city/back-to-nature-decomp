#ifndef OVL11_CONDITION_VALUE_QUERIES_H
#define OVL11_CONDITION_VALUE_QUERIES_H

#include "common.h"

/* Read-only classifier inputs in raw counter units, not percentages.
 * No arguments, state changes or allocations. Work state must be initialized.
 */

/* ovl_11:800EFF9C. Maximum signed counter at work+99EA+i*1D4 for i=1..36,
 * skipping Ovl11IsSpecialCounterSlot(i)==1. Starts at zero, so an empty
 * eligible set or all-negative counters returns zero.
 */
s16 Ovl11GetMaxEligibleRecordCounter(void);

/* ovl_11:800F002C. -1 while game flag 1B is set; otherwise return the first
 * i=0..4 with flag 1C+i set, or -1 if none. The return is an index, not ID.
 */
s32 Ovl11GetConditionFlagOverride(void);

/* ovl_11:800F00AC. Sum 25 UNSIGNED halfwords at work+498C, then add the
 * SIGNED halfword at work+44DC. The result may therefore be negative.
 */
s32 Ovl11GetConditionCounterTotal(void);

#endif
