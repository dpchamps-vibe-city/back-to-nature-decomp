#ifndef OVL11_SELECTION_PROMPT_STEPS_H
#define OVL11_SELECTION_PROMPT_STEPS_H

#include "common.h"
#include "game_types.h"

/* Step services over the GLOBAL interaction-step/resume protocol. Pass its
 * current step address and the caller's base step; out-of-window returns -1,
 * waiting returns 0, acceptance/completion 1, cancellation/rejection 3.
 * They mutate global steps, message window, portrait flag and draw mode;
 * they are not independent/reentrant state machines for arbitrary counters.
 * Descriptor aggregates retain their original by-value ABI and widths. */

/* Six steps: optional descriptor start message, record-selection input,
 * per-record message retry and descriptor cancellation message. descriptor
 * supplies record, start/cancel message pairs and selection draw mode. */
s32 Ovl11StepSelectionPrompt(s32 *step, s32 baseStep, Ovl11D8012D7B8 descriptor);

/* Six steps: initialize prompt from the selected ID's value, then edit its
 * count (cap 99). drawMode selects display; descriptor supplies selection
 * record and cancellation message, prompt receives the edited count at +8.
 * Acceptance here does not debit funds or transfer a record. */
s32 Ovl11StepQuantityPrompt(s32 *step, s32 baseStep, s32 drawMode,
                          Ovl11BAFCParams descriptor, ovl_11_DD48_arg0 *prompt);

/* Fifteen steps: selected-item eligibility/funds check, two confirmations,
 * request submission, completion wait, debit and progress action, then fade
 * out/relocate/fade in. Native destination is (1000,0,-2800); fade duration
 * is 20 updates each way. Entry must reference the current selection record.
 * Preserves intentional same-call fall-through into poll/draw phases. */
s32 Ovl11StepSelectedItemPurchase(s32 *step, s32 baseStep, StructD548 **record);

#endif
