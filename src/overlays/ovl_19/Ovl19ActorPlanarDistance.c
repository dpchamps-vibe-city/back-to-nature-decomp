/* Planar distance utility, original ovl_19:0x800BA564. */
#include "common.h"
#include "ovl19_actor_geometry.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 Ovl19ActorPlanarDistance(Ovl19ActorPlanarPosition *left,
                             Ovl19ActorPlanarPosition *right) {
    return SquareRoot0((left->position0 - right->position0) *
                       (left->position0 - right->position0) +
                       (left->position1 - right->position1) *
                       (left->position1 - right->position1));
}
