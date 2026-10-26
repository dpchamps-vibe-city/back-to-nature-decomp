#ifndef OVL15_SAVE_MENU_UI_H
#define OVL15_SAVE_MENU_UI_H

#include "common.h"

/* Overlay 15 only. Call once per UI update with valid render packet work,
 * relocated text banks and initialized menu state. These services draw in
 * screen pixels, enqueue sound effects and consume pad 0's input snapshot;
 * they do not poll the pad or perform memory-card I/O.
 *
 * Two-choice and slot menus share gOvl15MenuCursor. They process navigation
 * before accept/cancel; confirmation tests accept/cancel before navigation.
 * Initialize the cursor when switching menus (the confirmation choice has
 * its own state). Accept takes priority over cancel when both are present. */

/* Returns selected 0/1, -1 on cancel, or 2 while pending. */
s8 Ovl15UpdateTwoChoiceMenu(void);

/* Returns slot 0..2, -1 on cancel, or 3 while pending. displayMode is forwarded
 * to each summary; selection itself does not reject unavailable slots. */
s8 Ovl15UpdateSaveSlotMenu(s16 displayMode);

/* slot must be 0..2. displayMode 0 uses the common unavailable-slot text;
 * mode 1 distinguishes status 2 from 3. Other modes omit those messages.
 * Status 0 draws the saved date/names, sanitizes both eight-glyph names in
 * place and writes their ninth terminators. Uses shared text scratch and
 * text substitution slots 1..6: non-reentrant, not a read-only draw. */
void Ovl15DrawSaveSlotSummary(s16 slot, s16 displayMode);

/* Returns choice 0/1, -1 on cancel, or 2 while pending. Choice 0 uses the
 * acceptance sound; choice 1 uses the cancellation sound. */
s8 Ovl15UpdateConfirmationMenu(void);

/* Draws the prompt and returns waitingState until ANY pad-0 +8 input bit is
 * set, then plays the acceptance sound and returns pressedState. */
s32 Ovl15WaitForMenuButton(s16 waitingState, s16 pressedState);

#endif
