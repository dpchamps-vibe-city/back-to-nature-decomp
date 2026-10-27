/* Family-record query facade. Preserve the callable forwarding entry and
 * the lookup's raw count/index result; this is not a boolean predicate.
 * Membership and independent service boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_record_family_lookup.h"

s32 Ovl11QueryFamilyRecord(s16 family, s16 filter, s16 ordinal) {
    s32 result;

    result = Ovl11FindFamilyRecordIndex(family, filter, ordinal);
    return result;
}
