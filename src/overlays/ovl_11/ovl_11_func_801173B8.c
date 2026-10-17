#include "common.h"

s32 func_8002261C(s32 arg0, s32 arg1);
s32 func_800226A4(void);
void func_80022738(void);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
void ovl_11_func_8011764C(void);
s32 ovl_11_func_80117674(void);
s32 ovl_11_func_801176BC(u32 arg0);

s32 ovl_11_func_801173B8(void) {
    s32 message;
    s32 choice;

    switch (D_8012D794) {
    case 0:
    default:
        D_8012D52C = 0;
        func_80022738();
        return 1;
    case 1:
        if (D_80070CF6 == 6) {
            if (D_8012D798 != -1) {
                message = 0x11B;
                goto beginSelection;
            }
            message = 0x11A;
            D_8012D794 = 8;
            goto display;
        } else {
            message = 0x11A;
            D_8012D794 = 8;
            goto display;
        }
    beginSelection:
        D_8012D794 = 2;
        ovl_11_func_8011764C();
        D_8012D52C = 3;
    display:
        func_8002261C(2, message);
        return 0;
    case 2:
        if (func_800226A4() == 5) {
            D_8012D794 = 3;
        }
        break;
    case 3:
        switch (ovl_11_func_80117674()) {
        case 1:
            if (D_80071A5C >= 10) {
                ovl_11_func_800F2354(-10, 1);
                D_8012D794 = 4;
                message = 0x11C;
            } else {
                D_8012D794 = 8;
                message = 0x4F;
            }
            func_80022738();
            goto display;
        case 2:
            D_8012D794 = 0;
            break;
        }
        break;
    case 4:
        if (func_800226A4() == 2) {
            D_8012D794 = 5;
        }
        break;
    case 5:
        message = D_8012D798 + 0x11D;
        D_8012D794 = 6;
        ovl_11_func_8011764C();
        goto display;
    case 6:
        if (func_800226A4() == 5) {
            D_8012D794 = 7;
        }
        break;
    case 7:
        choice = ovl_11_func_80117674();
        switch (choice) {
        case 1:
            message = 0x4F;
            if (ovl_11_func_801176BC(D_8012D798) == 1) {
                message = 0x126;
            }
            D_8012D794 = 8;
            func_80022738();
            goto display;
        case 2:
            D_8012D794 = 8;
            func_80022738();
            func_8002261C(2, 0x127);
            break;
        }
        break;
    case 8:
        if (func_800226A4() == 2) {
            D_8012D794 = 0;
        }
        break;
    }
    return 0;
}
