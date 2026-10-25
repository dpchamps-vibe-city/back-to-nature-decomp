#include "common.h"
#include "game_types.h"
#include "ovl11_clock_calendar.h"

void ovl_11_func_800BFADC(Ovl11CounterPair *arg0);
s32 func_8001AF44(u32 arg0);
s32 ovl_11_func_800BFC20(s16 *arg0);
void ovl_11_func_800BFCA4(void);

s16 ovl_11_func_800BFB28(void) {
    Ovl11CounterPair next;
    Ovl11CounterPair current;

    next.unk0 = ((Ovl11BF8B8View *)D_8006C838)->field_44BA;
    current.unk0 = ((Ovl11BF8B8View *)D_8006C838)->field_44BA;
    next.unk2 = ((Ovl11BF8B8View *)D_8006C838)->field_44BC;
    current.unk2 = ((Ovl11BF8B8View *)D_8006C838)->field_44BC;
    ovl_11_func_800BFADC(&next);
    ((Ovl11BF8B8View *)D_8006C838)->field_E4C8 = ((Ovl11BF8B8View *)D_8006C838)->field_E4CA;
    if (Ovl11FindCalendarDate((s16)((Ovl11BF8B8View *)D_8006C838)->field_44BA, (s16)((Ovl11BF8B8View *)D_8006C838)->field_44BC) != 0) {
        ((Ovl11BF8B8View *)D_8006C838)->field_E4C8 = 0;
    }
    if ((Ovl11FindCalendarDate(next.unk0, next.unk2) != 0) && ((s16)((Ovl11BF8B8View *)D_8006C838)->field_E4C8 >= 3)) {
        ((Ovl11BF8B8View *)D_8006C838)->field_E4C8 -= 2;
    }
    if (func_8001AF44(0x2CU) == 1) {
        ((Ovl11BF8B8View *)D_8006C838)->field_E4C8 = 0;
    }
    if (ovl_11_func_800BFC20((s16 *) &current) == 1) {
        ((Ovl11BF8B8View *)D_8006C838)->field_E4C8 = 0;
    }
    ovl_11_func_800BFCA4();
    return (s16)((Ovl11BF8B8View *)D_8006C838)->field_E4C8;
}
