#include "common.h"
#include "screen_fade.h"

void ovl_21_func_800BB138(void);
void ovl_21_func_800BB250(void);
void func_8001B5DC(void);
void ovl_21_func_800BB2B4(void);
void ovl_21_func_800BA4C0(void);
s32 func_8001FE6C(void);
void func_8001FAE8(s32 arg0);
s32 func_8001FBBC(s16 arg0);
s32 func_80020B80(s32 arg0, s32 arg1);
void func_80011EF0(s32 arg0);

void ovl_21_func_800B8134(void) {
    s32 var_s0;

    ovl_21_func_800BB138();
    ovl_21_func_800BB250();
    func_8001B5DC();
    ovl_21_func_800BB2B4();
    ovl_21_func_800BA4C0();
    var_s0 = IsScreenFadeComplete() == 1;
    if (func_8001FE6C() == 0) {
        var_s0 = (var_s0 + 1) & 0xFF;
    }
    if (var_s0 == 2) {
        if (((Ovl21ExitStateView *) D_800C0448)->handles[0] != -1) {
            func_8001FAE8(((Ovl21ExitStateView *) D_800C0448)->handles[0]);
        }
        if (((Ovl21ExitStateView *) D_800C0448)->handles[1] != -1) {
            func_8001FAE8(((Ovl21ExitStateView *) D_800C0448)->handles[1]);
        }
        func_8001FBBC(0);
        func_80020B80(2, 0);
        func_80020B80(1, 0);
        ((Ovl21MainStateView *) D_8006C838)->state = 0xFF;
        func_80011EF0(6);
        ((Ovl21MainStateView *) D_8006C838)->flags |= 0x80000;
    }
}
