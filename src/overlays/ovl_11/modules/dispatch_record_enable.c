/*
 * dispatch_record_enable — reconstructed translation unit.
 *
 * Set the enable bit of the first matching kind in the thirteen-record
 * dispatch list.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_record_pool_refresh.h"
#include "ovl11_dispatch_record_enable.h"

void Ovl11SetDispatchRecordEnabled(s16 kind, s32 enabled) {
    Ovl11DispatchRecord *record;
    s32 *work;
    s32 *upperWork;

    work = (s32 *)&D_8006C838;
    upperWork = work + (0x8000 >> 2);
    record = Ovl11FindDispatchRecordByKind(kind, (Ovl11DispatchRecord *)upperWork[0x5DB4 >> 2], 13);
    if (record != 0) {
        if (enabled == 1) {
            record->flags |= 1;
        } else {
            record->flags &= 0xFFFE;
        }
    }
}
