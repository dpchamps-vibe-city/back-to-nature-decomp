#ifndef OVL11_DAILY_CONDITIONS_H
#define OVL11_DAILY_CONDITIONS_H

#include "common.h"

/* Resident ovl_11 calendar-dependent daily conditions (codes 0..4).
 * ChooseNext reads today's season/day and writes/returns tomorrow's code.
 * ApplyCurrent copies the previously chosen code to today, applies calendar,
 * record-date and flag-0x2C exclusions, then publishes flags 0x12..0x16.
 * Day-transition callers apply first, then choose: reversing the order would
 * consume the newly generated value immediately. Neither advances the clock.
 * Both use shared work memory; ChooseNext may consume the global RNG.
 *
 * Lookahead uses the original 0..29 day / 0..3 season rollover, without a
 * year. Calendar matches force code 0 or reduce codes >=3 by 2 on the day
 * before a match. The five record-date matches force code 0 as well.
 * Specific weather/gameplay labels for the codes are not assumed here. */
s16 Ovl11ChooseNextDayCondition(void);       /* ovl_11:800BF8B8 */
s16 Ovl11ApplyCurrentDayCondition(void);     /* ovl_11:800BFB28 */

/* 800BFD04: raw halfword store only. No validation or flag publication. */
void Ovl11SetCurrentDayCondition(s16 condition);

#endif
