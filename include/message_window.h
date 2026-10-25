#ifndef MESSAGE_WINDOW_H
#define MESSAGE_WINDOW_H

#include "common.h"

/* Shared, non-reentrant archive-backed message window. Call UpdateMessageWindow
 * once per scene update, with the render context and CD loader initialized.
 * Entry numbers select 0x2000-byte records in one of five archives; an archive
 * selector outside 0..4 uses archive zero. Reads reuse the global work area.
 * Text control codes, rather than this interface, determine interaction modes.
 */
s32 UpdateMessageWindow(void);

/* Return 0 if accepted, 1 otherwise. Idle starts immediately. While closing
 * (phase 5/6), one DIFFERENT request may be queued; there is no deeper queue.
 * Negative entry numbers and overflowing entry*0x2000 are not checked. */
s32 RequestMessageWindow(s32 archive, s32 entry);

/* Text-engine status, not the phase-dispatch index. 14 means loading/opening,
 * 15 closing; IsMessageWindowStatusFinal tests exactly status 2 or 5.
 * Choice is zero unless a text result 10..13 selected choice 1..4. */
s32 GetMessageWindowStatus(void);
s32 IsMessageWindowStatusFinal(void);
s32 GetMessageWindowChoice(void);

/* Close only advances phase 4 to 5, resets animation, and sets status 15.
 * Both calls restore the text/portrait gates. Reset restores saved flag
 * state, clears the phase and queue marker, and sets status 2; its finish
 * helper can copy queued IDs into the active IDs before that reset.
 * Neither operation cancels a pending CD transfer. */
void CloseMessageWindow(void);
void ResetMessageWindow(void);

/* Raw word, not normalized: only exactly 1 draws the portrait. Setters do not
 * clear the cached sprite or force a draw. */
void SetMessagePortraitEnabled(s32 enabled);
s32 GetMessagePortraitEnabled(void);

/* Only bit 0x08000000 of these saved work flags is restored at completion. */
void SetMessageWindowSavedWorkFlags(s32 flags);

/* Optional sprite prefix of a loaded record: first s32 is a text-slot value
 * (-1 means no sprite); sprite data starts at +4. Uploads it synchronously
 * through the existing sprite loader and installs the text slot. */
void LoadMessageWindowSprite(s32 *record);

#endif
