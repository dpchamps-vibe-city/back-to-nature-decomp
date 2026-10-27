#ifndef OVL11_CONDITION_EVALUATION_H
#define OVL11_CONDITION_EVALUATION_H

#include "common.h"

/* Installed 0x24-byte condition records are supplied by the existing
 * 800F0E00 installer (still separately compiled because of its unresolved
 * strength-reduction setting). Initialize work/calendar/object records,
 * flag banks and the result-slot service before these non-reentrant calls.
 * Packed mask and field contracts remain in ovl11_condition_masks.h and
 * ovl11_condition_record_fields.h; helpers retain external linkage.
 */

/* If work+10 bit 800 is set, return 1 WITHOUT consuming pending state.
 * Otherwise a nonzero evaluation state tests the installed record count in
 * order, with short-circuit predicates. Matching records attempt to reserve
 * the first result slot if absent; failures do not stop evaluation. Tests
 * may clear the pending halfword at work+51F0, and the shared current-record
 * pointer is overwritten for every visited record. Finally reset evaluation
 * state to zero and return 1, even when nothing matched or queued. */
s32 Ovl11EvaluatePendingConditions(void);

/* Accept state only if current state == 1 or game flag 2 == 0. Independently,
 * shared word 80070CF8 == 6 with a queued clock event discards any state but 8.
 * Thus even an ignored request can clear existing pending state. State codes
 * are compared by the condition records; no range validation is performed. */
void Ovl11SetConditionState(s32 state);

/* Attempt an ANY-slot result reservation for code (u16). Drop codes below
 * 12C when the shared 80070C88 word has mask 1 set. On success write code at
 * slot+2, the live work+25476 halfword at +4, and payload's low half at +6.
 * This wrapper discards the helper's success/failure result. */
void Ovl11QueueConditionCodeAnySlot(u16 code, s32 payload);

#endif
