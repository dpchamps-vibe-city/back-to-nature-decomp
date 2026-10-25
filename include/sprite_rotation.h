#ifndef SPRITE_ROTATION_H
#define SPRITE_ROTATION_H

#include "common.h"

/* ovl_11 only. States returned by Ovl11GetSpriteRotationState. */
enum {
    OVL11_ROTATION_FREE = 0,
    OVL11_ROTATION_IDLE = 1,
    OVL11_ROTATION_OUTBOUND = 2,
    OVL11_ROTATION_HOLD = 3,
    OVL11_ROTATION_RETURN = 4
};

/* Reset all ten records (including source pointers), and set ten game-state
 * halfwords at +0x443A to -1. Register sources before requesting rotations. */
void Ovl11ResetSpriteRotations(void);

/* Legacy s32 address of a pool with 50 records, stride 0x2C from +0x10.
 * Register records whose signed ID at +0xA is not 99, silently stopping new
 * registrations when all ten rotation slots are occupied. Source storage
 * must outlive its registration; registration does not deduplicate IDs. */
void Ovl11RegisterPoolSpriteRotations(s32 poolAddress);

/* Request by source ID. Return 1 when handled, 0 when not handled. Directions
 * used by a transition must be in 0..3 (hold-angle table index). A request
 * during HOLD adds five update ticks. Source flag 0x200 suppresses rotation.
 * IMPORTANT: request and lookup read source->id before testing state. A
 * matching ID must be encountered before any record with a NULL source. */
s32 Ovl11RequestSpriteRotation(s16 id, u16 direction);

/* Advance outbound/return angular steps by +/-8, hold for 11 update calls,
 * and mirror linked sprites when their flag 0x400 is set. Angle units are
 * 4096/turn. Game-state flag 0x08000000 suspends the entire update. */
void Ovl11UpdateSpriteRotations(void);

/* Raw state of the first matching source ID, or 0; same source-validity
 * requirement as request. ID comparison retains the legacy s32 parameter. */
u16 Ovl11GetSpriteRotationState(s32 id);

#endif
