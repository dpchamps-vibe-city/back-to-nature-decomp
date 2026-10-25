#ifndef OVL11_INTERACTION_STEPS_H
#define OVL11_INTERACTION_STEPS_H

#include "common.h"

/* Replace the shared current and resume step ids. Values are raw signed
 * words interpreted by the caller's state machine; no range validation. */
void Ovl11SetInteractionSteps(s32 current, s32 resume);

/* Store current+offset and resume+offset in the same two words. */
void Ovl11SetInteractionStepsOffset(s32 current, s32 resume, s32 offset);

/* Copy the saved resume id into the current step; leave resume unchanged. */
void Ovl11ResumeInteractionStep(void);

#endif
