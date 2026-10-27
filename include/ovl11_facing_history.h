#ifndef OVL11_FACING_HISTORY_H
#define OVL11_FACING_HISTORY_H

#include "common.h"

/* Save the low 16 bits of facing at shared work +0x5800, moving the previous
 * value to +0x5802. The direction controller supplies its current facing
 * (0..3) before selecting a new direction; this leaf does not validate it or
 * change the object. Repeated values still advance the two-value history.
 * Requires the shared work area to be initialized. No return value. */
void Ovl11PushFacingHistory(s32 facing);

#endif
