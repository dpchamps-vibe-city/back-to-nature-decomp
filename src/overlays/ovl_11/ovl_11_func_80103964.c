#include "common.h"

void ovl_11_func_80103EB8(void);
void ovl_11_func_80103F8C(void);
void ovl_11_func_801040A8(void);
s32 ovl_11_func_80103D44(void);
s32 ovl_11_func_80103C00(void);
void ovl_11_func_80103B24(void);
void ovl_11_func_800BCF28(void);
void func_80022738(void);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 GetVal8005E5B4(void);
void SetVal8005E334(s32 arg0);
void SetVal8005E2BC(s32 arg0);
s32 func_8001FABC(s16 arg0);

void ovl_11_func_80103964(void) {
    s32 result;

    ovl_11_func_80103EB8();
    ovl_11_func_80103F8C();
    ovl_11_func_801040A8();
    if (D_80127428 == 1) {
        result = ovl_11_func_80103D44();
        if (result == -1) {
            func_80022738();
            D_80127428 = 4;
        } else if (result == 1) {
            D_80127428 = 3;
        } else if (result == 2) {
            D_80127428 = result;
        }
    } else if (D_80127428 == 2) {
        result = ovl_11_func_80103C00();
        if (result == -1) {
            D_80127428 = 1;
            D_8012CF10[D_8012742C] = D_8012CF1C;
            D_8012CF20 = D_8012CF24;
        } else if (result == -2) {
            func_80022738();
            D_80127428 = 4;
        } else if (result == 1) {
            D_80127428 = result;
        } else if (result == 2) {
            func_80022738();
            D_80127428 = 3;
        }
    } else if (D_80127428 == 3) {
        func_80022738();
        func_8002261C(1, 0x448);
        ovl_11_func_80103B24();
        D_80127428 = 5;
        return;
    } else if (D_80127428 == 4) {
        func_80022738();
        func_8002261C(1, 0x447);
        ovl_11_func_80103B24();
        D_80127428 = 5;
        return;
    } else if (D_80127428 == 5) {
        if (GetVal8005E5B4() == 2) {
            ovl_11_func_800BCF28();
            func_8001FABC(3);
            D_80127428 = 0;
        }
        return;
    }
    SetVal8005E334(0);
    SetVal8005E2BC(0);
    func_8002261C(1, 0x764);
}
