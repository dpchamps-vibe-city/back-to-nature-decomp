#include "common.h"

void func_80017A38(s16 arg0, s16 arg1);
void func_80015114(u32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s16 arg6);
void ovl_11_func_80103964(void);

void ovl_11_func_801038E4(void) {
    if (D_80127428 != 0) {
        func_80017A38(0, 2);
        func_80015114((u32 *)(D_8005E3C0->field_D8 + 0x60), 0, 0, (s16)HWD0, (s16)VWD0, 0, 1);
        ovl_11_func_80103964();
        func_80017A38(0, 0);
    }
}
