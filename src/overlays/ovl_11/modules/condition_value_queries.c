/* Read-only inputs to Ovl11UpdateConditionValueCommand's classification:
 * an eligible-record maximum, a flag override and a counter total.
 * The three entries' only client is that command; no query owns storage.
 * This service cut does not assert recovered object boundaries under -G0.
 * The following map-percentage scan has its own private exclusion helper
 * and an unresolved compiler-setting constraint; see notes/file-groupings.md.
 */
#include "ovl11_condition_value_queries.h"
#include "ovl11_object_events.h"
#include "game_flags.h"

s16 Ovl11GetMaxEligibleRecordCounter(void) {
    s16 counter;
    s32 recordIndex;
    s16 maximum;
    char *recordBase;

    maximum = 0;
    recordIndex = 1;
    do {
        recordBase = (char *)D_8006C838 + recordIndex * 0x1D4;
        if (Ovl11IsSpecialCounterSlot((s16)recordIndex) != 1) {
            counter = *(s16 *)(recordBase + 0x8000 + 0x19EA);
            if (maximum < counter) {
                maximum = counter;
            }
        }
        recordIndex += 1;
    } while (recordIndex < 0x25);
    return maximum;
}

s32 Ovl11GetConditionFlagOverride(void) {
    s32 flagIndex;

    if (GetGameFlag(0x1B) == 1) {
        return -1;
    }
    for (flagIndex = 0; flagIndex < 5; flagIndex++) {
        if (GetGameFlag((u32)(flagIndex + 0x1C) << 16 >> 16) == 1) {
            return flagIndex;
        }
    }
    return -1;
}

s32 Ovl11GetConditionCounterTotal(void) {
    s32 total;
    s32 index;
    char *work;
    u16 *counter;

    total = 0;
    work = (char *)&D_8006C838;
    counter = (u16 *)(work + 0x498C);
    for (index = 0; index < 25; index++) {
        total += *counter++;
    }
    return total + ((Ovl11ConditionCounterTotalWork *)&D_8006C838)->additionalCounter;
}
