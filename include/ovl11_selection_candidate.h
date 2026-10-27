#ifndef OVL11_SELECTION_CANDIDATE_H
#define OVL11_SELECTION_CANDIDATE_H

#include "common.h"

/* Return 1 iff signed id matches an entry before the catalog's 0xE2 sentinel,
 * else 0. The catalog's u16 IDs compare after integer promotion, so a negative
 * id does not match. No selection/work state changes. */
s32 Ovl11SelectionCatalogContainsId(s16 id);

/* Scan the sentinel-terminated 0x20-byte candidate table in order. Required
 * flags/IDs and signed-byte adjustments are evaluated transactionally: shared
 * selection work IDs/flags are restored before each trial and on return.
 * Write candidate id/value and return its existing record index (0..79), or
 * 0xFF when not yet recorded. No eligible candidate returns 0xFE and writes
 * {id=0xE2,value=1}. Requires valid tables and writable output halfwords.
 * Not reentrant: the validators temporarily consume shared work state. */
s16 Ovl11ChooseEligibleSelection(u16 *outId, u16 *outValue);

/* Installed callback outcomes (messages 3:0x3C0..0x3C4). Wait for screen fade,
 * request the result message, and install the common exit on message status 2.
 * SameRecord commits immediately; Increase/Decrease commit only for choice 1.
 * Commit re-scans and writes the 0x16-byte record, work flags and eight IDs;
 * candidate availability and a free slot for a new id are caller invariants.
 * IDs D6/D7/D1 also set game flags 5B/5C/5D. */
void Ovl11SelectionUnchangedHandler(void);
void Ovl11SelectionSameRecordHandler(void);
void Ovl11SelectionNoCandidateHandler(void);
void Ovl11SelectionIncreaseHandler(void);
void Ovl11SelectionDecreaseHandler(void);

#endif
