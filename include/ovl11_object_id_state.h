#ifndef OVL11_OBJECT_ID_STATE_H
#define OVL11_OBJECT_ID_STATE_H

#include "common.h"
#include "game_types.h"

/* Original ovl_11:800D3468. Select a state from the object's leading ID,
 * then invoke Ovl11EnterObjectState. Always returns 0 (entry's veto result
 * is ignored). Some IDs consume one RNG draw; ID 15B uses clock hour and
 * signed field +AC. Unrecognized IDs report SystemError(45,64), then use
 * state 0 if error handling returns. Full writable object storage required. */
s32 Ovl11EnterObjectStateForId(u16 *object);

/* Original ovl_11:800D3574. Select an ID/season-specific sprite bank/offset.
 * Dynamic IDs 108/162/165/17A/17B use the script source selector; unavailable
 * sources or clock word == 6 clear the object's ID. Available dynamic sources
 * replace the embedded +0x78 source only when header +0x8C differs. Ordinary
 * sources initialize only when that header is zero. Unsupported ID/season
 * combinations call SystemError; they are not silently mapped to a fallback.
 * The object/source banks and calendar/script state must be initialized. */
void Ovl11RefreshObjectSpriteForId(u16 *object);

/* Original ovl_11:800D3BB0. Movement-step scalar for the motion updater,
 * NOT a duration: ID 15B -> 24, 162/165 -> 6, everything else -> 12.
 * Pure lookup; only the object's leading halfword is read. */
s32 Ovl11GetObjectMoveStep(u16 *object);

/* The four weighted choices remain published by ovl11_random_state_choice.h.
 * They and the clock/value helper keep their original external linkage. */

#endif
