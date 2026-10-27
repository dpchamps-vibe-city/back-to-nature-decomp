#ifndef OVL11_SELECTED_RECORD_PAIR_COMMANDS_H
#define OVL11_SELECTED_RECORD_PAIR_COMMANDS_H

#include "common.h"

/* Four operations exposed together by object opcode 12 of E9D28. Work,
 * selected indices and record tables must be initialized; no allocation.
 * The lower-level record_pair_slots service remains a separate interface. */

/* Restore the two pair banks, bank 1 before bank 0. */
void Ovl11RestoreSelectedRecordPairs(void);

/* selector must be 0..3 with valid pair indices. Selectors 1/3 return 0
 * when availability +44D0/+44D2 is zero. For 0/1, test flag 200 hex on the
 * SECOND indexed B8-byte record; for 2/3, test type 10A hex on the FIRST
 * indexed B4-byte record. Returns 1/0. Does not change records. */
s32 Ovl11IsSelectedRecordPairMarked(s16 selector);

/* Start/poll required text entry over the shared 18-byte status scratch.
 * On text-entry status 1, copy all 18 bytes to the FIRST selected record:
 * 0/1 -> B8-stride pool, 2/3 -> B4-stride pool; other selectors -> scratch
 * itself. Returns 1 only for that copy, otherwise 0. A shared non-reentrant
 * text-entry service is used; this does not preload scratch from the target. */
s32 Ovl11EditSelectedRecordText(s16 selector);

/* For each marked pair, clear its slot. For 0/1 first clear second-record
 * flag 200 and first-record flag 02000000 hex. Then resync row placements
 * in bank order 1, 0. The availability guards above still apply. */
void Ovl11ClearMarkedRecordPairs(void);

#endif
