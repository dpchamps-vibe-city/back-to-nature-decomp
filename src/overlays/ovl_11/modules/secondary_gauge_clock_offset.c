/* Signed secondary-gauge to clock-offset conversion (ovl_11:800CD4E4).
 * Supplies the extra minute offset used by primary_object_clock_advance;
 * the exact signed ranges and out-of-range results are in the public header.
 */
#include "ovl11_secondary_gauge_clock_advance.h"
#include "game_types.h"

s32 Ovl11GetSecondaryGaugeClockOffset(Ovl11SecondaryGaugeView *object) {
    s16 secondaryGauge;
    s32 offsetMinutes;

    secondaryGauge = object->secondaryGauge;
    if (secondaryGauge < 0x32) {
        return 0;
    }
    if (secondaryGauge < 0x46) {
        return 0x78;
    }
    if (secondaryGauge < 0x50) {
        return 0xF0;
    }
    if (secondaryGauge < 0x64) {
        return 0x168;
    }
    offsetMinutes = 0;
    return offsetMinutes;
}
