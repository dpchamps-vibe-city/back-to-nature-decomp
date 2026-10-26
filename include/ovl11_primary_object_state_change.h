#ifndef OVL11_PRIMARY_OBJECT_STATE_CHANGE_H
#define OVL11_PRIMARY_OBJECT_STATE_CHANGE_H

#include "common.h"
#include "game_types.h"

/* Save previous state (+3E), install newState (+3C) and the low halfword of
 * animationParameter (+1C), set change bit 0x20000000 and clear 0x04000000.
 * Select the low-byte animation, then apply flag-selected variants, optional
 * source-bank reload and saved-object animation synchronization. Flag 0x1000
 * suppresses ordinary selection; 0x800 requests reload. Some variants modify
 * the state or frame and can request an actuator sequence. No return value.
 * Callers supply a valid object with sprite state at +260. */
void Ovl11ChangePrimaryObjectState(Ovl11C7010Object *object, s32 newState,
                                   s32 animation, s32 animationParameter);

/* The direct path saves/sets the same state and flags but selects only the
 * requested low-byte animation, without variants or source-bank reload.
 * Only if its frame-control high bit is set does it store animationParameter
 * at +1C and the facing low byte as the frame. Returns 0 when that bit is
 * clear, otherwise the facing word (which may itself be zero): NOT a Boolean
 * success result. Keeps the existing general object view and word ABI. */
s32 Ovl11ChangeObjectStateDirect(Recon_ovl_11_func_800C7270_A0View *object,
                                 s32 newState, s32 animation, s32 animationParameter);

#endif
