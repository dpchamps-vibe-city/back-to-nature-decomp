#include "common.h"
#include "message_window.h"

void ovl_21_func_800BA4C0(void);
s32 ovl_21_func_800B97F4(void);
/* Original overlay imports for SetVal8005E2BC and SetMessagePortraitEnabled. */
void SetVal8005E2BC(s32 arg0);

void ovl_21_func_800B90C4(void) {
    s32 slot;
    s32 next;

    ovl_21_func_800BA4C0();
    slot = ovl_21_func_800B97F4();
    if (slot == -1) {
        RequestMessageWindow(4, 0x26);
        next = 0xC;
    } else {
        next = 0xB;
        if (slot == 0) {
            next = 0xA;
        }
        SetVal8005E2BC(0);
        SetMessagePortraitEnabled(0);
        RequestMessageWindow(4, D_800BCCD4[((Ovl21SelectorView *)D_800C0448)->selectors[slot]]);
    }
    ((Ovl21SelectorView *)D_800C0448)->field_988 = 0;
    D_800C0448[0] = next;
}
