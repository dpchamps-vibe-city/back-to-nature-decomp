#include "common.h"

s32 func_80012A34(s32 arg0);
void ovl_11_func_8011FD2C(s16 arg0);
void ovl_11_func_8011FD7C(s32 arg0);
void ovl_11_func_8011FDCC(s16 arg0);

s32 ovl_11_func_8011FB80(s16 arg0) {
    s16 var_s0;
    s16 var_a1;
    s16 *cnt;

    if (arg0 == 4) {
        return 0x10A;
    }
    if (arg0 == 3) {
        s16 *p;
        s32 n;

        p = D_80128580_ary;
        n = func_80012A34(2) + 1;
        return p[D_80128554 * 3 + n];
    }
    cnt = D_80071A70;
    var_a1 = D_80128550_ary[arg0 - 1];
    if (cnt[var_a1] == 0) {
        var_s0 = 0x1E9;
    } else if (cnt[var_a1] == 1) {
        var_s0 = D_8012855C[var_a1].unk4;
    } else {
        var_s0 = D_8012855C[var_a1].unk2;
    }
    if (var_s0 != 0x1E9) {
        cnt[D_80128550_ary[arg0 - 1]] = cnt[D_80128550_ary[arg0 - 1]] - 1;
        switch (D_80128550_ary[arg0 - 1]) {
        case 0:
            ovl_11_func_8011FD2C(1);
            ovl_11_func_8011FD7C(0xC8);
            return var_s0;
        case 1:
            ovl_11_func_8011FD2C(1);
            ovl_11_func_8011FD7C(0x64);
            return var_s0;
        case 2:
            ovl_11_func_8011FDCC(1);
            return var_s0;
        case 3:
            ovl_11_func_8011FD2C(0xA);
            ovl_11_func_8011FD7C(0x1F4);
            return var_s0;
        case 4:
            ovl_11_func_8011FDCC(1);
            break;
        }
    }
    return var_s0;
}
