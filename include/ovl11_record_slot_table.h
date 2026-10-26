#ifndef OVL11_RECORD_SLOT_TABLE_H
#define OVL11_RECORD_SLOT_TABLE_H

#include "common.h"

/* Reset phase+five six-halfword slots, active pointer, 128/8 tracked counters
 * (FF = idle), an adjacent 128-byte work block, and the original counter seeds.
 * Save-image code persists the same work-area storage. */
void Ovl11InitRecordSlotTable(void);

/* Unless work+10 bit 1000 blocks service, publish slot 0 as active. An empty
 * slot clears flag 2 and the associated request bits. Otherwise service its
 * tag, dropping a stale mode-0 slot or completing its handler. The original
 * handler table has ONE entry (tag 0); no out-of-range tag check is made.
 * The handler receives its table-word address (the installed handler ignores
 * it). Completion clears phase and flag 2 and shifts the queue; waiting sets
 * flag 2. Phase, tags, and payload are not interchangeable. */
void Ovl11ServiceActiveRecordSlot(void);

/* Return a writable six-halfword slot or NULL. IfAbsent rejects an existing
 * matching nonfree tag anywhere, then tests only slot 0. Any scans all five
 * without duplicate rejection. On success ALL SIX halfwords become zero;
 * tag is only a duplicate-test key and is NOT installed. Callers fill payload
 * at +2/+4/+6, including the mode read by stale-slot detection. */
s16 *Ovl11ReserveFirstRecordSlotIfAbsent(s32 tag);
s16 *Ovl11ReserveAnyRecordSlot(s32 tag);

/* Non-FF counters increment then clamp at 120. The first 128 always advance;
 * the secondary eight advance only after calendar/object/state gates pass.
 * No elapsed-time scaling; one invocation is one increment. */
void Ovl11AdvanceTrackedRecordCounters(void);

/* Test only queued slots 1..4, excluding the active slot 0. */
s32 Ovl11HasQueuedRecordSlots(void);

#endif
