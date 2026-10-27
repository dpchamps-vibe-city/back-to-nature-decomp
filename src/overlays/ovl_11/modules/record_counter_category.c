/* Five-candidate counter category, ovl_11:800F02C8.
 * Public contract: ovl11_record_counter_category.h.
 * Membership/boundaries: notes/file-groupings.md. */
#include "common.h"
#include "ovl11_record_counter_category.h"

s32 Ovl11SelectHighestRecordCounter(void) {
    char *record;
    u16 counter;
    s32 candidate;
    s32 bestCandidate;
    s32 maximum;

    maximum = 0;
    bestCandidate = 0;
    candidate = 0;
    for (; candidate < 5; candidate++) {
        record = (char *)&D_8006C838 + gOvl11CounterCategoryRecordIds[candidate] * 0x1D4;
        counter = *(u16 *)(record + 0x8000 + 0x19EC);
        if (maximum < counter) {
            bestCandidate = candidate;
            maximum = counter;
        }
    }
    if (maximum >= 0x1F4) {
        return bestCandidate;
    }
    return -1;
}
