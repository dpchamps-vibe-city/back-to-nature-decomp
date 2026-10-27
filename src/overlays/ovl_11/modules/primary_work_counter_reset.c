/*
 * primary_work_counter_reset — reconstructed translation unit.
 *
 * Clear primary-object flag 30 and three adjacent shared work counters during
 * area entry and day-transition reset. The shared work buffer remains
 * external.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_primary_work_counter_reset.h"

void Ovl11ClearPrimaryWorkCounters(void) {
    char *work = (char *)&D_8006C838;

    *(s32 *)(work + 0x5234) &= 0xBFFFFFFF;
    *(s32 *)(work + 0x52D8) = 0;
    *(s32 *)(work + 0x52DC) = 0;
    *(s32 *)(work + 0x52E0) = 0;
}
