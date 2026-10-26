#ifndef OVL11_OPTIONS_MENU_H
#define OVL11_OPTIONS_MENU_H

#include "common.h"

/* Resident overlay-11 pause UI. Requires initialized render context, pad
 * input and shared message window. Neither UI is reentrant; do not run both
 * at once. Drawing consumes packet-work storage and ordering-table entries.
 * Cursor positions and panel dimensions are screen pixels. */

/* If inactive, reset all selectors, copy twelve settings bytes from work
 * +E4CC, enter the root menu and play sound 3. Active calls do nothing. */
void Ovl11OpenOptionsMenu(void);

/* Draw/update an active options menu. Root has two pages: a two-column
 * description list (five/six rows) and four settings rows (three choices in
 * row zero, two otherwise). Closing waits for message status 2, commits the
 * saved/edited bytes to work and applies actuators/stereo/clock HUD settings.
 * Sound/clock choices preview immediately; enabling actuators can run a test.
 * The two-byte selector views preserve their original wrap/narrowing rules. */
void Ovl11DrawOptionsMenu(void);

/* Apply the three work halfwords at +E4CE/+E4D0/+E4D2: any nonzero value
 * enables actuators, stereo and clock HUD respectively. No menu state change. */
void Ovl11ApplyWorkSettings(void);

/* Reset row to zero and phase to FF (inactive), without clearing result. */
void Ovl11ResetMenuChoiceState(void);

/* Reset then enter phase zero (accept input). Existing result remains until
 * the next service call writes a new one. */
void Ovl11BeginMenuChoice(void);

/* Draw/input while phase=0; result is 0 while waiting, -1 on cancel, or
 * 1..4 for the chosen row. Nonzero result enters FE, waiting for the message
 * window to finish, then FF. State/result are the canonical globals declared
 * by common.h. Choices 1/2 start a position transition (2 also sets flag 48);
 * 3/4 request scene submodes F/11 with ten-step screen/audio fades. The input
 * frame initiates these effects; the closing phase does not repeat them. */
void Ovl11ServiceMenuChoice(void);

#endif
