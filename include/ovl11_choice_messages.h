#ifndef OVL11_CHOICE_MESSAGES_H
#define OVL11_CHOICE_MESSAGES_H

#include "common.h"

/* Arm only when idle. The updater runs only while the far-state halfword
 * at +2 is 2, builds four {message entry, image variant} slots from the shared
 * work area, and uses message archive 3. Pad directions select slots 0..3;
 * input 0x20 closes the interaction. Requires the message-window service,
 * selected-context fields and sprite-image load buffer to be initialized. */
void Ovl11RequestChoiceMessages(void);
void Ovl11UpdateChoiceMessages(void);

/* Both clear phase and selected slot. They do not close a message, restore
 * images or undo the shared cell-state changes. Reset is the retained wrapper
 * used by other interaction drivers; Clear is the dispatcher's own reset. */
void Ovl11ResetChoiceMessages(void);
void Ovl11ClearChoiceMessages(void);

#endif
