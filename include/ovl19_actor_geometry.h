#ifndef OVL19_ACTOR_GEOMETRY_H
#define OVL19_ACTOR_GEOMETRY_H

#include "common.h"
#include "game_types.h"

/* Integer planar distance from the signed halfwords at record +0x10/+0x14,
 * in the records' position units, using SquareRoot0(dx*dx + dy*dy).
 * No state changes or overflow checks; squared differences and their sum
 * must fit the signed 32-bit arithmetic. No vertical component is read. */
s32 Ovl19ActorPlanarDistance(Ovl19ActorPlanarPosition *left,
                             Ovl19ActorPlanarPosition *right);

#endif
