#ifndef OVL25_BACKGROUND_COLOR_CYCLE_H
#define OVL25_BACKGROUND_COLOR_CYCLE_H

#include "common.h"

/* Set both render contexts' background RGB and enable automatic clearing.
 * Components are narrowed to bytes without clamping. No GPU submission. */
void Ovl25SetBackgroundColor(s16 red, s16 green, s16 blue);

/* ovl_25 must be resident. Interpolate the two RGB triples with denominator
 * 511 at the current signed index, set the background, then advance one
 * position and reverse at 0/511. The endpoint is held for two updates.
 * Uses mutable private cycle state; no elapsed-time input and no reset. */
void Ovl25StepBackgroundColorCycle(void);

#endif
