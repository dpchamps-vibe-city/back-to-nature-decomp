#ifndef OVL11_RECORD_FLAG_COUNTERS_H
#define OVL11_RECORD_FLAG_COUNTERS_H

#include "common.h"

/* Disable label lookup (index=-1) and set game flag 1B. The original
 * zero-length memset does NOT initialize the counters or selected record. */
void Ovl11InitRecordFlagCounters(void);

/* Select an event-record index (caller must supply a valid 0..36 index).
 * IDs 3/7/9/14 select labels 0/1/2/3; other IDs select label 4. Clear work
 * +E76C and snapshot eight bytes from work +44B8 to +E796. */
void Ovl11SelectCounterRecord(s16 recordIndex);

/* Copy work +44BA/+44BC to +DBDC/+DBDE, clear flag 4E and counter +E770. */
void Ovl11ResetRecordThresholdCount(void);

/* No-op unless flag 41 is set. Consume/clear flags 4B/4C; advance the
 * selected record's threshold/low-value counters, setting 4E/4D/AF as
 * appropriate; with flag 4F set, advance the stage count and flags 50..52.
 * Counts are update invocations, not seconds. Selection must already be valid.
 * All operations mutate shared work/flags; they are not reentrant. */
void Ovl11UpdateRecordFlagCounters(void);

/* Return the selected label pointer in the original s32 ABI. Call only after
 * selection, not while the signed label index is -1. No bounds check. */
s32 Ovl11GetCounterRecordLabel(void);

#endif
