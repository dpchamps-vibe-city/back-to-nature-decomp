#include "common.h"

s32 func_80013394(void);
s32 func_8002261C(s32 arg0, s32 arg1);
/* GetVal8005E5B4 in the executable, named by this overlay's symbol map. */
s32 func_800226A4(void);
void ovl_11_func_801035C0(void);
void ovl_11_func_80103714(void);

void ovl_11_func_80103424(void) {
    if (func_80013394() != 0) {
        func_8002261C(3, 0x3C1);
        if (func_800226A4() == 2) {
            ovl_11_func_801035C0();
            Ovl11_D_801273DC = ovl_11_func_80103714;
        }
    }
}
