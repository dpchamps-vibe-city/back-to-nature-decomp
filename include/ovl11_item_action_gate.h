#ifndef OVL11_ITEM_ACTION_GATE_H
#define OVL11_ITEM_ACTION_GATE_H

#include "common.h"

/* Gate the primary object's current item action. inputMode == 0 uses the
 * mode-1/mode-6 cell probe, item flags/status and shared dispatch flags;
 * any nonzero inputMode takes the separate item-class path. No allocation.
 * Return 1 when input is handled, NOT necessarily when a new state starts
 * (cell class 2 can consume it without transition); otherwise return 0.
 * May change object state/animation/flags and play sounds 4A/4B. The zero
 * path leaves gOvl11ProbedObjectCell pointing at the selected grid cell or
 * NULL on unavailable/out-of-range lookup. Requires valid object item index,
 * loaded item table, area grids and the shared interaction state. */
s32 Ovl11TryBeginItemAction(s32 inputMode);

#endif
