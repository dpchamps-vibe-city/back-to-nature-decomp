#ifndef OVL19_ACTOR_CONTROLS_H
#define OVL19_ACTOR_CONTROLS_H

#include "common.h"
#include "game_types.h"

/* ovl_19:800BAC40 / 800BAC50. Store state and parameters into a valid actor
 * control prefix, truncating each word to s16. The shorter operation leaves
 * extra unchanged; both preserve the leading halfword. These are raw state
 * writes, not queued transitions. Parameters are interpreted by each actor's
 * update service (direction/countdown/etc.), not uniformly by these writers. */
void Ovl19SetActorControl(Ovl19Func800BAC40Arg *control, s32 state, s32 parameter, s32 extra);
void Ovl19SetActorState(Ovl19Func800BAC40Arg *control, s32 state, s32 parameter);

/* ovl_19:800BAC5C / 800BAC7C. Set/clear sprite-animation pause bit 4 on
 * three actor sprites at stride 0x40 and two auxiliary sprites at stride
 * 0x48. No other flag, animation cursor or actor state is changed. Requires
 * the overlay's sprite groups to be initialized. Calls also replace the
 * stored flag-operation callback. They do not themselves advance animation. */
void Ovl19PauseActorSprites(void);
void Ovl19ResumeActorSprites(void);

/* ovl_19:800BAC9C. Common implementation: zero pauses, every nonzero value
 * resumes. Retains external linkage; callers normally use the wrappers. */
void Ovl19SetActorSpritePauseMode(s32 resume);

#endif
