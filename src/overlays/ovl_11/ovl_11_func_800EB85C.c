#include "common.h"

s32 ovl_11_func_800EDF3C();
s32 ovl_11_func_800E5ADC(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E5B84(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E5870(s16 arg0, s16 arg1, s32 arg2, s16 arg3);

s32 ovl_11_func_800EB85C(s16 arg0) {
    s16 i;
    s16 state;
    s32 result;
    s32 start;

    state = *(s16 *)&D_80129560[arg0];
    start = ovl_11_func_800EDF3C(D_80124984[0], 0, 0, 0) != 1;
    result = 0;
    switch (state) {
    case 0:
        for (i = start; i < 5; i++) {
            ovl_11_func_800E5ADC(D_80124984[i], 0, D_80124990[0][i], D_80124990[1][i]);
            ovl_11_func_800E5B84(D_80124984[i], 5, 0, 1);
        }
        result = 1;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        for (i = start; i < 5; i++) {
            result = ovl_11_func_800E5B84(D_80124984[i], 10, 0, 0);
        }
        break;
    case 5:
        for (i = start; i < 3; i++) {
            result = ovl_11_func_800E5870(D_80124984[i], 0, 500, 11);
            if (result == 1) {
                ovl_11_func_800E5B84(D_80124984[i], 5, 0, 1);
            }
        }
        for (; i < 5; i++) {
            result = ovl_11_func_800E5870(D_80124984[i], 2, 2200, 6);
            if (result == 1) {
                ovl_11_func_800E5B84(D_80124984[i], 5, 0, 11);
            }
        }
        break;
    case 6:
        result = ovl_11_func_800E5B84(D_80124984[1], 12, 0, 0);
        break;
    case 7:
        ovl_11_func_800E5B84(D_80124984[1], 5, 0, 1);
        result = ovl_11_func_800E5B84(D_80124984[3], 12, 0, 0);
        if (result == 1) {
            state += start;
        }
        break;
    case 8:
        ovl_11_func_800E5B84(D_80124984[3], 5, 0, 1);
        result = ovl_11_func_800E5B84(D_80124984[0], 12, 0, 0);
        break;
    case 9:
        ovl_11_func_800E5B84(D_80124984[3], 5, 0, 1);
        ovl_11_func_800E5B84(D_80124984[0], 5, 0, 1);
        result = ovl_11_func_800E5B84(D_80124984[4], 12, 0, 0);
        break;
    case 10:
        ovl_11_func_800E5B84(D_80124984[4], 5, 0, 1);
        result = ovl_11_func_800E5B84(D_80124984[2], 12, 0, 0);
        break;
    case 11:
        for (i = start; i < 3; i++) {
            result = ovl_11_func_800E5870(D_80124984[i], 2, 1350, 11);
            if (result == 1) {
                ovl_11_func_800E5B84(D_80124984[i], 5, 0, 1);
            }
        }
        for (; i < 5; i++) {
            result = ovl_11_func_800E5870(D_80124984[i], 0, 1350, 11);
            if (result == 1) {
                ovl_11_func_800E5B84(D_80124984[i], 5, 0, 1);
            }
        }
        break;
    case 12:
        for (i = start; i < 5; i++) {
            result = ovl_11_func_800E5B84(D_80124984[i], 13, 0, 0);
        }
        break;
    case 13:
    case 14:
    case 15:
        for (i = start; i < 5; i++) {
            result = ovl_11_func_800E5B84(D_80124984[i], 5, 0, 0);
        }
        break;
    case 16:
        for (i = start; i < 3; i++) {
            result = ovl_11_func_800E5870(D_80124984[i], 2, D_80124990[1][i], 8);
            if (result == 1) {
                ovl_11_func_800E5B84(D_80124984[i], 5, 0, 1);
            }
        }
        for (; i < 5; i++) {
            result = ovl_11_func_800E5870(D_80124984[i], 0, D_80124990[1][i], 6);
            if (result == 1) {
                ovl_11_func_800E5B84(D_80124984[i], 5, 0, 1);
            }
        }
        break;
    }
    if (result == 1) {
        if (state == 16) {
            state = 1;
        } else {
            state++;
        }
    }
    D_80129560[arg0] = state;
    return 1;
}
