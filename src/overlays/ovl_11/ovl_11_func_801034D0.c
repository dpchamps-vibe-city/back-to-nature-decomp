#include "common.h"

s32 func_80013394(void);
s32 func_8002261C(s32 arg0, s32 arg1);
/* Executable imports GetVal8005E5B4 and GetVal8005E5B8. */
s32 func_800226A4(void);
s32 func_800225B8(void);
void ovl_11_func_801035C0(void);
void ovl_11_func_80103714(void);

void ovl_11_func_801034D0(void) {
    if (func_80013394() != 0) {
        func_8002261C(3, 0x3C3);
        if (func_800226A4() == 2) {
            if ((s16)func_800225B8() == 1) {
                ovl_11_func_801035C0();
            }
            Ovl11_D_801273DC = ovl_11_func_80103714;
        }
    }
}
