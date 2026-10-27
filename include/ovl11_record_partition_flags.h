#ifndef OVL11_RECORD_PARTITION_FLAGS_H
#define OVL11_RECORD_PARTITION_FLAGS_H

#include "common.h"

/* Operate on all sixteen 0x18-byte records in initial pool partition 6.
 * Requires BindStateDataTables' pool layout. No allocation or ID checks. */

/* Exactly 1 clears flag 0x4000 (enabled); any other value sets it (disabled).
 * All other flags and record contents are preserved. */
void Ovl11SetPartition6Enabled(s32 enabled);
/* Count flag-clear entries, regardless of their IDs or other flags: 0..16. */
s32 Ovl11CountEnabledPartition6Records(void);

#endif
