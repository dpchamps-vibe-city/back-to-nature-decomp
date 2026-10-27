#ifndef OVL11_PRIMARY_OBJECT_LEVEL_BANDS_H
#define OVL11_PRIMARY_OBJECT_LEVEL_BANDS_H

#include "common.h"
#include "game_types.h"

/* Refresh +36 band flags from signed levels +12/+16 and maximum +14.
 * Low bands use (maximum / 10) * {1,3,5}, with integer truncation before
 * multiplication; the second level uses exact 100 and thresholds 80/50.
 * Newly entered bands choose a state, with later tests taking precedence.
 *
 * If suppressStateChange is zero, a chosen state sets +6C bits 0x5800 and
 * enters that state with parameters (0,-2), returning 1. Otherwise return 0.
 * Suppression still updates the flags and drains the GLOBAL active-record
 * slot on entry to the exact-100 band. It is not a side-effect-free query.
 * The object must provide all fields above; values are not clamped.
 */
s32 Ovl11UpdatePrimaryObjectLevelBands(Ovl11C7010Object *object, s32 suppressStateChange);

#endif
