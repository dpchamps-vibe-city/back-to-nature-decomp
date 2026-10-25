#ifndef OVL17_TIMER_HUD_H
#define OVL17_TIMER_HUD_H

#include "common.h"

/* ovl_17:800BB0C4. Private u32 tick counter and elapsed-time HUD.
 * mode -1: reset only; 0: advance and draw; 1: draw without advancing;
 * 2: advance without drawing; other modes: no effect.
 *
 * Formatting assumes 30 ticks/second and displays minute-within-hour,
 * seconds and two fractional digits. Advancing is per call, not a VSync
 * measurement; call frequency determines the real elapsed-time scale.
 * Drawing requires the active render context and packet/text renderers.
 * No return value, bounds/capacity check or automatic timer wrap/reset. */
void Ovl17UpdateTimerHud(s16 mode);

#endif
