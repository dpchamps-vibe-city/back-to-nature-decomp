#ifndef OVL11_OBJECT_EFFECT_OFFSET_H
#define OVL11_OBJECT_EFFECT_OFFSET_H

#include "common.h"

/* Screen-space Y offset, in pixels, for the timed effect attached to a
 * rendered object. objectId is the signed halfword ID from the object record.
 * Selected IDs return -10, -20, -30 or -40; all others return zero.
 * This does not move the object's main sprite or marker and changes no state.
 * The implementation retains the original signed low-halfword normalization
 * relative to ID 0xA1. No gameplay meaning is assigned to the ID classes. */
s32 Ovl11GetObjectEffectYOffset(s32 objectId);

#endif
