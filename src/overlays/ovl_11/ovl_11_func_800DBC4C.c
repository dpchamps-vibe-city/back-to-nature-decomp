#include "common.h"
#include "ovl11_clock_calendar.h"

s32 ovl_11_func_800EF0E8(s16 arg0);

void ovl_11_func_800DBC4C(void) {
    s32 i;

    for (i = 0; i < 0x80; i++) {
        if (D_8007AE78[i] != 0xFF) {
            D_8007AE78[i]++;
            D_8007AE78[i] = D_8007AE78[i] >= 0x79 ? 0x78 : D_8007AE78[i];
        }
    }
    if (Ovl11FindCalendarDate(
            ((struct Ovl11TimerState *) D_8006C838)->field_44BA,
            ((struct Ovl11TimerState *) D_8006C838)->field_44BC) != 0) {
        return;
    }
    for (i = 0; i < 5; i++) {
        if (ovl_11_func_800EF0E8(i) == 2) {
            return;
        }
    }
    if (ovl_11_func_800EF0E8(-1) != 2 &&
        (u32) (((struct Ovl11TimerState *) D_8006C838)->field_E4C8 - 3) >= 2U) {
        for (i = 0; i < 8; i++) {
            if (D_8007AEF8[i] != 0xFF) {
                D_8007AEF8[i]++;
                D_8007AEF8[i] = D_8007AEF8[i] >= 0x79 ? 0x78 : D_8007AEF8[i];
            }
        }
    }
}
