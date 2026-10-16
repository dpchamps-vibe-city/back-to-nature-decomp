#include "common.h"

s32 func_80013394(void);
void func_800223B0(s32 arg0);
void func_800132B8(s32 arg0, s32 arg1, s32 arg2);
void ovl_11_func_800FBE88(void);
void ovl_11_func_800FC1AC(void);
void ovl_11_func_800BCF28(void);

void ovl_11_func_800FC054(void) {
    s32 temp_s0;

    temp_s0 = func_80013394();
    if (D_8012720C >= 3 && D_8012720C <= 5) {
        func_800223B0(D_8005E3C0->field_D8 + 0x74);
    }
    switch (D_8012720C) {
    case 1:
        if (temp_s0 == 1) {
            D_8012720C = 2;
            ((struct struct_8006C838_view *) D_8006C838)->field_0C |= 0x20000;
            return;
        }
    default:
        return;
    case 2:
        ovl_11_func_800FBE88();
        func_800132B8(0xA, 0, 2);
        D_8012720C = 3;
        return;
    case 3:
        if (temp_s0 == 1) {
            D_8012720C = 4;
            return;
        }
        break;
    case 4:
        ovl_11_func_800FC1AC();
        return;
    case 5:
        if (temp_s0 == 1) {
            func_800132B8(0xA, 0, 2);
            D_8012720C = 6;
            ((struct struct_8006C838_view *) D_8006C838)->field_0C |= 0x40000;
        }
        break;
    case 6:
        if (temp_s0 == 1) {
            D_8012720C = 0;
            ovl_11_func_800BCF28();
        }
        break;
    }
}
