/* Paired current/resume interaction-step operations, ovl_11:8011D084..8011D0C8.
 * These are raw state-word operations, not dispatch or validation. The same
 * pair is written by both setters and consumed by resume. Membership and
 * evidence-limited responsibility boundaries are in notes/file-groupings.md. */
#include "ovl11_interaction_steps.h"

void Ovl11SetInteractionSteps(s32 current, s32 resume) {
    gOvl11InteractionStep = current;
    gOvl11InteractionResumeStep = resume;
}

void Ovl11SetInteractionStepsOffset(s32 current, s32 resume, s32 offset) {
    gOvl11InteractionStep = current + offset;
    gOvl11InteractionResumeStep = resume + offset;
}

void Ovl11ResumeInteractionStep(void) {
    gOvl11InteractionStep = gOvl11InteractionResumeStep;
}
