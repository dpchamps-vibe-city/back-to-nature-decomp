#ifndef OVL11_COUNTER_CHOICE_H
#define OVL11_COUNTER_CHOICE_H

#include "common.h"

/* One shared, non-reentrant message-choice event. Begin saves and clears the
 * work area's selection halfword, resets local state and starts phase 1.
 * Its legacy return value is the work-area address plus 0x8000, not a handle.
 * Update is called once per service tick; inactive updates do nothing. It
 * temporarily selects text context 3 and restores the caller's context.
 *
 * Choices use two highest categories from six signed counters and one random
 * text row. Counts must allow at least two nonnegative category indices.
 * The event coordinates messages, screen/audio fades and CD track 25, then
 * restores the saved selection, clears the object's 0x400 flag, returns the
 * area to mode 2 and sets game flag 0x76. No independent instances are allowed.
 * Delay values count service calls, not milliseconds. */
char *Ovl11BeginCounterChoice(void);
void Ovl11UpdateCounterChoice(void);

/* Clears local phase/text/indices only; it does NOT perform event completion
 * or restore the saved work-area selection. */
void Ovl11ResetCounterChoice(void);

#endif
