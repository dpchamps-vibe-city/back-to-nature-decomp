#include "common.h"
#include "ovl11_clock_calendar.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_area_record_claims.h"

s32 ovl_11_func_8010CAD4(u8 *arg0) {
    if ((u32) (((Ovl11CAD4StateView *)D_8006C838)->field_E4C8 - 3) >= 2U) {
        if (Ovl11IsClockAdvanceActive() != 1) {
            return 0;
        }
        if ((u32) (((Ovl11CAD4StateView *)D_8006C838)->field_44C0 - 6) >= 0xFU) {
            return 0;
        }
        if ((*(u16 *) ((u8 *) arg0 + 0xB8)) & 0x40) {
            return 0;
        }
        Ovl11DispatchAreaRecordAction(arg0);
    }
    return 0;
}
