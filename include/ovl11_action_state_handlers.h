#ifndef OVL11_ACTION_STATE_HANDLERS_H
#define OVL11_ACTION_STATE_HANDLERS_H

#include "common.h"
#include "game_types.h"

/* Entry-phase callbacks selected by gOvl11ActionEntryHandlers (19 slots).
 * Slot order: keyframes 0,1,2; null; motion; found/probed/handoff; keyframes
 * 8,8,10,11,12,13,14,15,16; motion; movement. Numeric suffixes are bank slots,
 * not gameplay names. The bank retains its raw address-word representation
 * in globals_override.h; callers select a valid slot and initialized object.
 *
 * The keyframe callbacks use +28 as phase/cursor, +2A as update count and
 * +34 bit 800 as a sequence lock. Phase 0 starts, -1 reports completion;
 * most completions also set +2C to 300 update ticks (slot 2 does not).
 * Reaching the end of a script sets phase -1 but returns 0 until the next
 * call. Compound handlers also mutate shared record/timer state, movement,
 * direction, and sometimes sound. These globals make them non-reentrant.
 * Do not call them as independent stateless animation predicates.
 */
s32 Ovl11StepActionKeyframes0(Ovl11D04D4Obj *object);
s32 Ovl11StepActionKeyframes1(Ovl11D04D4Obj *object);
s32 Ovl11StepActionKeyframes2(Ovl11D04D4Obj *object);
s32 Ovl11StepActionKeyframes8(Ovl11D04D4Obj *object);
s32 Ovl11StepActionKeyframes10(Ovl11D04D4Obj *object);
s32 Ovl11StepActionKeyframes11(Ovl11D04D4Obj *object);
s32 Ovl11StepActionKeyframes12(Ovl11D04D4Obj *object);
s32 Ovl11StepActionKeyframes13(Ovl11D04D4Obj *object);
s32 Ovl11StepActionKeyframes14(Ovl11D04D4Obj *object);
s32 Ovl11StepActionKeyframes15(Ovl11D04D4Obj *object);
s32 Ovl11StepActionKeyframes16(Ovl11D04D4Obj *object);
s32 Ovl11StepFoundRecordAction(Ovl11A47CObject *object);
s32 Ovl11StepProbedRecordAction(Ovl11A654Object *object);
s32 Ovl11StepActionRecordHandoff(Ovl11A47CObject *object);

/* Both return 1 after updating movement. Refresh selects animations from one
 * of two three-halfword rows according to gOvl11ActionMotionVariant; it can
 * retain the lock and resets the 300-tick timer only outside flag 2000.
 */
s32 Ovl11StepActionMovement(Recon_ovl_11_func_800D2594_A0View *object);
s32 Ovl11RefreshActionMotion(Recon_ovl_11_func_800D2594_A0View *object);

#endif
