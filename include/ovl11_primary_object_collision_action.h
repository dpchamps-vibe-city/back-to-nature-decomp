#ifndef OVL11_PRIMARY_OBJECT_COLLISION_ACTION_H
#define OVL11_PRIMARY_OBJECT_COLLISION_ACTION_H

#include "common.h"
#include "game_types.h"

/* Probe center XYZ plus the direction/probeIndex pair in Ovl11CollisionOffsets,
 * in engine position units (no conversion). Both indices are unchecked.
 * Clears flag 40000, updates action/contact flags and may request state 1
 * on the global primary object. Qualifying hit flags 2/4/200 increment the
 * shared dispatch count until 51, then dispatch action D to their handlers.
 * Held button 20 removes query-mask bit 80. The shared collision budget applies.
 * Publishes hit pointer/flags except for probes 0/38, which normally clear
 * both; probe 38 publishes when hit flag 4000 is set. Retain these effects
 * even when object flags later force the return value to zero.
 * Return 0/2 for clear/blocked, 4 for hit flag 4000, or 999 for that flag on
 * probe 38, narrowed to s16. Flag 8 or flag 80 with mode 39/3A forces 0. */
s32 Ovl11QueryPrimaryObjectCollisionAction(Ovl11CB114Object *object,
    s32 *center, s16 probeIndex);

#endif
