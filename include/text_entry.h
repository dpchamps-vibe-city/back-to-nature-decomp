#ifndef TEXT_ENTRY_H
#define TEXT_ENTRY_H

#include "common.h"

/* Two shared, non-reentrant input services. Initialize the pad input records,
 * active render context, text resources and message-window service first.
 * Updates draw a semitransparent backdrop and drive message-window prompts;
 * run at most one service at a time, once per input/frame update.
 *
 * Both update calls return 0 when idle, 2 while active, -1 on cancellation,
 * or 1 on confirmation. A terminal result clears the service's state, so the
 * next update returns 0. They read pad 0's pressed/repeat button streams.
 */

/* recordAddress retains the existing s32-address ABI. The record's category
 * u16 at +2 and item-index u16 at +4 are updated before final confirmation;
 * cancellation does not roll back those writes. The signed cursor selects
 * four categories, then thirty items laid out in seven columns. */
void BeginIndexedSelection(s32 recordAddress);
s32 UpdateIndexedSelection(void);
void ResetIndexedSelection(void);

/* buffer stores up to eight u16 character codes. Begin calls fill the first
 * nine elements with 0xFFFF; they do not edit in place. The on-grid confirm
 * key also writes buffer[length+1], so reserve TEN elements for every path.
 * Results are left in that same buffer. 0x0FFD is the blank code; confirmation
 * requires a nonempty buffer with at least one nonblank character entered.
 * Backspacing does not recompute that historical nonblank-entry flag.
 * Required disallows cancelling from an empty buffer. AlternateLayout also
 * chooses the alternate preview formatting; it still allows cancellation.
 */
void BeginTextEntry(u16 *buffer);
void BeginRequiredTextEntry(u16 *buffer);
void BeginTextEntryAlternateLayout(u16 *buffer);
s32 UpdateTextEntry(void);

/* Clears editor state but neither erases nor unbinds the caller's buffer. */
void ResetTextEntry(void);

#endif
