#include "common.h"

void ovl_21_func_800B8320(void);
void ovl_21_func_800BA4C0(void);
s32 func_8002261C(s32 arg0, s32 arg1);
/* Executable alias of GetVal8005E5B4. */
s32 func_800226A4(void);
void func_80022738(void);

void ovl_21_func_800B86B8(void) {
    u8 *base;

    ovl_21_func_800B8320();
    ovl_21_func_800BA4C0();
    func_8002261C(4, 0x19);
    if (func_800226A4() == 2) {
        func_80022738();
        base = (u8 *)D_800C0448;
        *(s16 *)(base + 0x988) = 0;
        *(s16 *)base = 3;
    }
}
