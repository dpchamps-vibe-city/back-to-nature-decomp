#include "common.h"

void ovl_21_func_800B8320(void);
void ovl_21_func_800BA4C0(void);
void func_80022738(void);
/* Executable aliases of SetVal8005E2BC and SetVal8005E334. */
void SetVal8005E2BC(s32 arg0);
void SetVal8005E334(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);

void ovl_21_func_800B8A24(void) {
    u8 *base;

    ovl_21_func_800B8320();
    ovl_21_func_800BA4C0();
    func_80022738();
    SetVal8005E2BC(0);
    SetVal8005E334(0);
    func_8002261C(4, 0x1A);
    base = (u8 *)D_800C0448;
    *(s16 *)(base + 0x988) = 0;
    *(s16 *)base = 6;
}
