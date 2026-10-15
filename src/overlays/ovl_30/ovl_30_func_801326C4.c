#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 ovl_30_func_801326C4(s32 arg0) {
    u32 i;
    u32 index;
    struct Ovl30DayCounters *data;

    if (*(s32 *)D_8005E3A8 & 0x2000) {
        D_80134AA8 += 10;
        if (D_80134AA8 >= 0x80) {
            D_80134AA8 = 0;
        }
    }
    if (*(s32 *)D_8005E3A8 & 0x8000) {
        D_80134AA8 -= 10;
        if (D_80134AA8 <= 0) {
            D_80134AA8 = 0x7F;
        }
    }
    if (*(s32 *)D_8005E3A8 & 0x4000) {
        D_80134AA8++;
        if (D_80134AA8 >= 0x80) {
            D_80134AA8 = 0;
        }
    }
    if (*(s32 *)D_8005E3A8 & 0x1000) {
        if (D_80134AA8 <= 0) {
            D_80134AA8 = 0x7F;
        } else {
            D_80134AA8--;
        }
    }
    if (*(s32 *)D_8005E3A8 & 0x20) {
        ((struct Ovl30DayCounters *)D_8006C838)->counters[D_80134AA8]++;
        if (((struct Ovl30DayCounters *)D_8006C838)->counters[D_80134AA8] >= 0xFF) {
            ((struct Ovl30DayCounters *)D_8006C838)->counters[D_80134AA8] = 0;
        }
    }
    if (*(s32 *)D_8005E3A8 & 0x40) {
        if (((struct Ovl30DayCounters *)D_8006C838)->counters[D_80134AA8] == 0) {
            ((struct Ovl30DayCounters *)D_8006C838)->counters[D_80134AA8] = 0xFF;
        } else {
            ((struct Ovl30DayCounters *)D_8006C838)->counters[D_80134AA8]--;
        }
    }
    if (*(s32 *)D_8005E3A8 & 0x10) {
        ((struct Ovl30DayCounters *)D_8006C838)->counters[D_80134AA8] += 10;
        if (((struct Ovl30DayCounters *)D_8006C838)->counters[D_80134AA8] >= 0xFF) {
            ((struct Ovl30DayCounters *)D_8006C838)->counters[D_80134AA8] = 0;
        }
    }
    if (*(s32 *)D_8005E3A8 & 0x80) {
        if (((struct Ovl30DayCounters *)D_8006C838)->counters[D_80134AA8] == 0) {
            ((struct Ovl30DayCounters *)D_8006C838)->counters[D_80134AA8] = 0xFF;
        } else {
            ((struct Ovl30DayCounters *)D_8006C838)->counters[D_80134AA8] -= 10;
        }
    }
    data = (struct Ovl30DayCounters *)D_8006C838;
    FntPrint(D_8012EED0, D_80134AA8, data->counters[D_80134AA8]);
    i = 0;
    do {
        index = D_80134AA8 + i;
        if (index < 0x80U) {
            FntPrint(D_8012EEE4, (s32)index, data->counters[index]);
        }
        i++;
    } while (i < 10U);
    return arg0;
}
