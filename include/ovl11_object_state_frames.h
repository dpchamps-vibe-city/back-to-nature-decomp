#ifndef OVL11_OBJECT_STATE_FRAMES_H
#define OVL11_OBJECT_STATE_FRAMES_H

#include "common.h"
#include "game_types.h"

/* Frame callbacks for decimal slots of the 21-entry object-state bank;
 * slot 5 is null. Entry gates are published in ovl11_object_state_callbacks.h.
 * Call through the dispatcher with a valid live record of the appropriate
 * family. These are partial views, not allocation sizes.
 *
 * Sequences use phase +28, ticks +2A, outer timer +2C and flag +34 bit 800
 * (hex). Initialization/termination may change direction, motion, shared
 * progress flags, audio, record selection or record lifetime. Keyframe tables
 * are packed {s16 code,duration} rows terminated by code -1; no bounds checks.
 * Timings are update calls, not milliseconds. Coordinate units are retained
 * from the record/motion services, with no conversion here.
 *
 * Return values are 0 while a sequence runs and commonly 1 on its completion
 * path, but the dispatcher IGNORES them. Motion/follow callbacks return 1
 * every call. Completion is communicated through record flags/state/timers.
 * The common keyframe-completion path marks phase -1 first; the NEXT call
 * releases flag 800 and arms timer 300, or clears the record as documented
 * by its callback body (states 02/03/04/14/15/16 clear the record).
 * State06 deliberately pre-decrements +2C, then passes THAT same cell to the
 * keyframe stepper instead of +2A. Shared work/banks make this non-reentrant. */
s32 Ovl11StepState00Keyframes(Ovl11D04D4Obj *object);
s32 Ovl11StepState01Motion(u16 *object);
s32 Ovl11StepState02CollisionSequence(Ovl11D4314Object *object);
s32 Ovl11StepState03DirectedKeyframes(Ovl11DirectedKeyframeObject *object);
s32 Ovl11StepState04KeyframesAndClear(Ovl11D04D4Obj *object);
s32 Ovl11StepState06TimedSprite(Ovl11D55F8Object *object);
s32 Ovl11StepState07RecordPlacement(Ovl11RecordPlacementStateObject *object);
s32 Ovl11StepState08Keyframes(Ovl11D04D4Obj *object);
s32 Ovl11StepState09Keyframes(Ovl11D04D4Obj *object);
s32 Ovl11StepState10Keyframes(Ovl11D04D4Obj *object);
s32 Ovl11StepState11Keyframes(Ovl11D04D4Obj *object);
s32 Ovl11StepState12Keyframes(Ovl11D04D4Obj *object);
s32 Ovl11StepState13Keyframes(Ovl11D04D4Obj *object);
s32 Ovl11StepState14DirectedKeyframes(Ovl11DirectedKeyframeObject *object);
s32 Ovl11StepState15DirectedKeyframes(Ovl11DirectedKeyframeObject *object);
s32 Ovl11StepState16DirectedKeyframes(Ovl11DirectedKeyframeObject *object);
s32 Ovl11StepState17Keyframes(Ovl11D04D4Obj *object);
s32 Ovl11StepState18RecordSelection(Ovl11SetFieldsView *object);
s32 Ovl11StepState19ExitMotion(Ovl11SetFieldsView *object);
s32 Ovl11StepState20RecordFollow(u16 *object);

#endif
