#ifndef OVL11_RECORD_POOL_SETUP_H
#define OVL11_RECORD_POOL_SETUP_H

#include "common.h"

/* Apply the 0x20-set/0x40-clear enable-bit rules to all nineteen installed
 * dispatch-record lists, in list order, then set partition 6's disable bit
 * 0x4000 on all sixteen entries. Requires initialized list/count tables at
 * shared work+0xDD8C/+0xDDD8. Each signed count bounds its own list;
 * nonpositive counts skip that list. No allocation or bounds validation. */
void Ovl11ApplyPoolRecordFlagRules(void);

/* Update list-10 gates, then assign the row-selected values across all
 * installed lists. The clock client passes its live season as row (0..3);
 * only row assignment consumes that argument. Its table index is unchecked.
 * In live season 0 only, state 0xD or 0x38..0x42 on the current pool record
 * publishes/resets its position; the same state range at work+0x91DC installs
 * selector 1 and position (-1800,0,3100) on gStageMode1Record. This is not a pure flag
 * update: those delegated services also mutate record/state fields.
 * Requires the initialized shared work, list tables and current record. */
void Ovl11SetupRecordRowsAndGates(s16 row);

/* Forward the current signed season to the list-10 gate service. That
 * service reads live season/variant/progress itself and ignores its argument;
 * retain the forwarding call's ABI without inventing a new selector meaning. */
void Ovl11RefreshCurrentDispatchRecordGates(void);

#endif
