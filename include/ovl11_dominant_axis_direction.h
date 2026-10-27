#ifndef OVL11_DOMINANT_AXIS_DIRECTION_H
#define OVL11_DOMINANT_AXIS_DIRECTION_H

#include "common.h"

/* ovl_11:0x800F06D8. target and origin each point to signed X/Y/Z words
 * in the same coordinate units; Y is ignored and neither input is changed.
 * Return 0=-Z, 1=-X, 2=+Z, 3=+X along the larger absolute displacement.
 * Ties choose Z; coincident positions return 0. Differences and their
 * absolute values must fit s32. No null checks or persistent state.
 */
s32 Ovl11GetDominantXZDirection(s32 *target, s32 *origin);

#endif
