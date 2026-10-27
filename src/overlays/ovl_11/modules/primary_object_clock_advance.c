/* Gauge-dependent clock-advance request (ovl_11:800CD624).
 * The area-mode state machine calls this before its menu-exit transition.
 * Snapshot the object's clock boundary before starting the clock transition.
 */
#include "ovl11_secondary_gauge_clock_advance.h"
#include "game_types.h"
#include "ovl11_clock_calendar.h"

void Ovl11ApplySecondaryGaugeClockAdvance(void) {
    char *object;
    char *work;
    s32 offsetMinutes;

    object = (char *)&gPrimaryObjectBase;
    offsetMinutes = Ovl11GetSecondaryGaugeClockOffset((Ovl11SecondaryGaugeView *)object);
    /* Retain the original sentinel fallback, although this classifier's
     * current range contains no -1 result. */
    if (offsetMinutes == -1) {
        offsetMinutes = 0;
    }
    /* The original two-base expression reaches D_8006C838. Save its clock
     * boundary halfword at the primary object's +2C before requesting time. */
    work = object - 0x51C8;
    *(u16 *)(work + 0x51F4) = *(u16 *)(work + 0x44CA);
    Ovl11RequestClockAdvanceToSix(offsetMinutes);
}
