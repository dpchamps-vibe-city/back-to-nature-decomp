#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 ovl_30_func_80132978(s32 arg0) {
    u32 i;
    u32 index;
    struct Ovl30DayCounters1 *data;

    if (*(s32 *)gPad0Input & 0x2000) {
        D_80134AAC += 10;
        if (D_80134AAC >= 8) {
            D_80134AAC = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x8000) {
        D_80134AAC -= 10;
        if (D_80134AAC <= 0) {
            D_80134AAC = 7;
        }
    }
    if (*(s32 *)gPad0Input & 0x4000) {
        D_80134AAC++;
        if (D_80134AAC >= 8) {
            D_80134AAC = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x1000) {
        if (D_80134AAC <= 0) {
            D_80134AAC = 7;
        } else {
            D_80134AAC--;
        }
    }
    if (*(s32 *)gPad0Input & 0x20) {
        ((struct Ovl30DayCounters1 *)D_8006C838)->counters[D_80134AAC]++;
        if (((struct Ovl30DayCounters1 *)D_8006C838)->counters[D_80134AAC] >= 0xFF) {
            ((struct Ovl30DayCounters1 *)D_8006C838)->counters[D_80134AAC] = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x40) {
        if (((struct Ovl30DayCounters1 *)D_8006C838)->counters[D_80134AAC] == 0) {
            ((struct Ovl30DayCounters1 *)D_8006C838)->counters[D_80134AAC] = 0xFF;
        } else {
            ((struct Ovl30DayCounters1 *)D_8006C838)->counters[D_80134AAC]--;
        }
    }
    if (*(s32 *)gPad0Input & 0x10) {
        ((struct Ovl30DayCounters1 *)D_8006C838)->counters[D_80134AAC] += 10;
        if (((struct Ovl30DayCounters1 *)D_8006C838)->counters[D_80134AAC] >= 0xFF) {
            ((struct Ovl30DayCounters1 *)D_8006C838)->counters[D_80134AAC] = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x80) {
        if (((struct Ovl30DayCounters1 *)D_8006C838)->counters[D_80134AAC] == 0) {
            ((struct Ovl30DayCounters1 *)D_8006C838)->counters[D_80134AAC] = 0xFF;
        } else {
            ((struct Ovl30DayCounters1 *)D_8006C838)->counters[D_80134AAC] -= 10;
        }
    }
    data = (struct Ovl30DayCounters1 *)D_8006C838;
    FntPrint(D_8012EED0, D_80134AAC, data->counters[D_80134AAC]);
    i = 0;
    do {
        index = D_80134AAC + i;
        if (index < 8U) {
            FntPrint(D_8012EEFC, (s32)index, data->counters[index]);
        }
        i++;
    } while (i < 10U);
    return arg0;
}
