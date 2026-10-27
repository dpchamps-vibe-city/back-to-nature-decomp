/*
 * seasonal_record_gates — reconstructed translation unit.
 *
 * Forward the current season to the list-10 dispatch-record gate service.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_record_pool_setup.h"
#include "ovl11_dispatch_record_flags.h"

void Ovl11RefreshCurrentDispatchRecordGates(void) {
    Ovl11UpdateList10RecordGates(gCalendarSeason);
}
