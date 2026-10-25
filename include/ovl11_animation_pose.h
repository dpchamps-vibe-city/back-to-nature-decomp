#ifndef OVL11_ANIMATION_POSE_H
#define OVL11_ANIMATION_POSE_H

#include "common.h"
#include "game_types.h"

/* Object-handler callback: without flag 0x2000, clear the sequence lock,
 * arm timer +2C at 300 and return 1. Otherwise clear +2E, invoke the pose
 * service below and return 0. Mutates the caller's object, not module state. */
s32 Ovl11SubmitDeferredAnimationPose(Recon_ovl_11_func_800E0220_A0View *object);

/* Select period 2/6 from +B2 and mode 0x8030 for handler ID 13, otherwise
 * 0x8031; call the common object-update helper, maintain flag 0x04000000,
 * derive animation +24 from the +48/+4C/+50 words and clear lock 0x800.
 * Returns 1. The second argument is retained but ignored. Requires the
 * shared work-area handler state to be initialized. */
s32 Ovl11SubmitAnimationPose(Recon_ovl_11_func_800D2594_A0View *object, s32 unused);

#endif
