#ifndef TWO_CHOICE_PROMPT_H
#define TWO_CHOICE_PROMPT_H

#include "common.h"

/* Queue a 40x36 or 80x36 pixel beveled prompt at (x,y), using the current text
 * bank. selection chooses the cursor row (14-pixel pitch); no bounds check.
 * Both reset text cursor offsets to (0,0), rather than restoring old offsets.
 * Requires initialized text/sprite data, ordering table and packet space. */
void Ovl11DrawCompactChoicePrompt(s32 ot, s16 x, s16 y, s16 selection);
void Ovl11DrawWideChoicePrompt(s32 ot, s16 x, s16 y, s16 selection);

/* Process pad-0 PRESSED bits, priority up/down/confirm/cancel. Edits selection
 * as unsigned halfword bits, tests it as signed and wraps to 0 or 1. Returns
 * 1 on confirm/cancel, else 0; cancel forces row 1. Plays movement/choice SFX.
 * Exposed also to the separate compact-prompt controller. */
s32 Ovl11ProcessTwoChoiceInput(u16 *selection);

/* Draw then process the persistent wide prompt: 0=pending, 1=row 0 accepted,
 * 2=row 1 accepted/cancel. Completion resets selection to 0. x==-1/-2 only
 * initializes selection to 0/1 and returns 0 without drawing or processing.
 * Normal coordinates are screen pixels. Non-reentrant shared prompt state. */
s32 Ovl11UpdateWideChoicePrompt(s32 ot, s16 x, s16 y);

#endif
