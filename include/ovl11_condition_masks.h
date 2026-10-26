#ifndef OVL11_CONDITION_MASKS_H
#define OVL11_CONDITION_MASKS_H

#include "common.h"

/* Read-only tests of a packed condition word; all return exactly 0 or 1.
 * Season codes 0..3 select bits 30..27; weekday codes 0..6 select bits
 * 26..20. An out-of-range calendar code fails its test. These are required
 * allow-lists: a zero field does NOT mean unrestricted.
 *
 * Current daily-condition codes 0,1,2,3,4 select bits 19,13,18,12,11.
 * All five bits set (0xC3800) bypass the state read, even for an invalid
 * current code; otherwise an unrecognized code fails. The code is the shared
 * work halfword at +0xE4C8, published by ovl11_daily_conditions.h's service.
 * No specific weather interpretation is assumed. */
s32 Ovl11MaskAllowsSeason(s32 conditions);
s32 Ovl11MaskAllowsWeekday(s32 conditions);
s32 Ovl11MaskAllowsDayCondition(s32 conditions);

/* Require all three allow-lists above. Bits 17 and 16 additionally request
 * the existing record queries (0,0,1,0) and (0,1,0,0), respectively; a zero
 * query result rejects the word. Tests short-circuit in that order. Other
 * bits are ignored here. No clock, condition or record state is modified. */
s32 Ovl11TestConditionMask(s32 conditions);

#endif
