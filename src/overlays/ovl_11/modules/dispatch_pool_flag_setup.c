/*
 * dispatch_pool_flag_setup — reconstructed translation unit.
 *
 * Apply the enable-mask rules to all nineteen installed dispatch-record
 * lists, then disable partition 6.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_record_pool_setup.h"
#include "ovl11_record_partition_flags.h"
#include "ovl11_dispatch_record_flags.h"

void Ovl11ApplyPoolRecordFlagRules(void) {
    char *work;
    Ovl11DispatchRecord **lists;
    s16 *counts;
    s32 index;

    work = (char *)&D_8006C838;
    lists = (Ovl11DispatchRecord **)(work + 0xDD8C);
    counts = (s16 *)(work + 0xDDD8);
    for (index = 0; index < 19; index++) {
        Ovl11ApplyDispatchRecordFlagRules(*counts, *lists);
        lists++;
        counts++;
    }
    Ovl11SetPartition6Enabled(0);
}
