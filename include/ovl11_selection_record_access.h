#ifndef OVL11_SELECTION_RECORD_ACCESS_H
#define OVL11_SELECTION_RECORD_ACCESS_H

#include "game_types.h"

/* ovl_11 must be resident. Records have ten occupied/nonzero leading words,
 * a signed index at +5C, entry IDs at +28 and message IDs at +46.
 * StructD548 and Ovl11D548Arg are established word-aligned 0x60-byte views
 * of this protocol; keep their distinct by-value ABIs. Normal indices 0..9
 * are required. Accessors do not validate indices or record contents. */

/* Read pad 0 repeat/press streams. Up/down chooses a nonempty slot, wrapping
 * across ten entries (or retaining the old index if none of the other nine
 * is occupied). Navigation has priority over action buttons and plays sound
 * 5 even when the index stays put. Returns 0 for navigation/no action, 1 for
 * button 0x40, 2 for 0x10 (disabled in draw mode 7), 3 for 0x20, with their
 * existing sound effects. Mutates only record.index, plus sound state. */
s32 Ovl11ProcessSelectionRecordInput(Ovl11D548Arg *record);

/* Search a copy of the record: direction==1 advances, every other value
 * retreats. The direction word follows the whole record on the stack. */
s16 Ovl11FindOccupiedSelectionIndex(Ovl11D548Arg record, s32 direction);

/* Count nonzero leading words strictly before index, for compressed row
 * placement. Returns zero when index<=0; preserves the Q16 counter arithmetic.
 * No state changes. */
s32 Ovl11CountPrecedingSelectionEntries(StructD548 record);

/* Read the currently selected signed entry ID or message ID. No mutation;
 * a negative value (including the caller-tested -1 message) is preserved. */
s16 Ovl11GetSelectionEntryId(StructD548 record);
s16 Ovl11GetSelectionMessageId(StructD548 record);

#endif
