#ifndef OVL11_SELECTION_MESSAGE_H
#define OVL11_SELECTION_MESSAGE_H

#include "common.h"

/* ovl_11 must be resident. A single, non-reentrant request slot selects one
 * of ten state-dependent message families. Uses the shared text driver;
 * callers must serialize requests with other users of that driver. */

/* Cancel the pending request and clear its category; leaves variant bytes
 * and repeat counters unchanged. Original identity: ovl_11:0x8011F0C4. */
void Ovl11ResetSelectionMessage(void);

/* Latch category clamped to 0..9 only while idle; requests while busy are
 * discarded, not queued. Original identity: ovl_11:0x8011F0D8. */
void Ovl11RequestSelectionMessage(s32 category);

/* Service only when the current scene header's kind is 0x15. Advance the
 * category's byte state, submit text command 2 with the selected message code,
 * wait for text status 2, then release the slot on the following eligible
 * call. Other scene kinds pause the request. No elapsed-time input.
 * Original identity: ovl_11:0x8011F114. */
void Ovl11UpdateSelectionMessage(void);

#endif
