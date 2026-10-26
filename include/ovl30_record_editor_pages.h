#ifndef OVL30_RECORD_EDITOR_PAGES_H
#define OVL30_RECORD_EDITOR_PAGES_H

#include "common.h"

/* Descriptor-based debug page callbacks. Input/output is a row cursor;
 * when the fade is incomplete, return it unchanged. The page draws/edits
 * the current record before pad bits 4/8 step its record selector, so a new
 * selection is displayed on the next invocation. Requires resident game
 * work/save records, initialized font state and pad 0.
 *
 * Cow/sheep: 25 rows, 20 records, 0xB8 stride; chicken: 23 rows, 10 records,
 * 0xB4 stride; coop chicken: 23 rows, 3 records, 0xB4 stride; resident: 32
 * rows, 37 records, 0x1D4 stride. Animal selectors are words; the resident
 * selector is a signed halfword in the shared work area.
 *
 * Descriptor addresses, widths, flags and limits are preserved literally,
 * including repeated resident row addresses. Label translations do not
 * resolve every pre-existing field-name/layout interpretation. These pages
 * are diagnostic editors, not a validated gameplay mutation API. */
s32 Ovl30EditCowSheepData(s32 cursor);
s32 Ovl30EditChickenData(s32 cursor);
s32 Ovl30EditCoopChickenData(s32 cursor);
s32 Ovl30EditResidentData(s32 cursor);

/* Same fade/cursor protocol: seven 0xF8-stride Korobokkuru records, 29 rows
 * including five shared work-area halfwords; system flags use a static
 * 12-row mask table. No field reinterpretation is implied by label names. */
s32 Ovl30EditKorobokkuruData(s32 cursor);
s32 Ovl30EditSystemFlagBits(s32 cursor);

/* These two pages do NOT wait for the screen fade. The game-flag page edits
 * the selected index in 0..1183 and displays ten flags; displayed numbers
 * can exceed the range while lookups wrap. Returns handlerState unchanged.
 * The object-event page scrolls/displays ten of 37 slots and returns zero;
 * the callback table's handler-state argument is deliberately unread. */
s32 Ovl30EditGameFlagIndex(s32 handlerState);
s32 Ovl30DrawObjectEventSlots(void);

#endif
