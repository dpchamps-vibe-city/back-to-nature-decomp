#include "common.h"

/* The original call sites explicitly clear both unused trailing slots. */
s32 ovl_11_func_800E5C60();
s32 ovl_11_func_800EDF3C();
s32 ovl_11_func_800E5ADC(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E5B84(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800F07EC(s16 arg0, s32 arg1, s32 arg2, u32 *arg3);

s32 ovl_11_func_800EB010(s16 arg0, s16 arg1) {
    u16 value = *(u16 *)&D_80129560[arg1];
    struct Ovl11IdList ids = D_800B9B1C;
    u32 state;
    s32 selection;
    s32 i;
    s16 *p;
    s32 *x;
    s32 *y;
    s16 *z;

    ids.values[1][4] = value;
    switch ((s16)(value - 3)) {
    case 17: selection = 0; break;
    case 0: selection = 1; break;
    case 11: selection = 2; break;
    case 4: selection = 3; break;
    case 6: selection = -1; break;
    default: selection = 4; break;
    }
    if (selection != -1) {
        ovl_11_func_800E5C60(ids.values[0][selection], 1, 0, 0);
        ids.values[0][selection] = 8;
        ids.values[1][selection] = 9;
    } else {
        ovl_11_func_800E5C60(8, 1, 0, 0);
    }
    if (ovl_11_func_800EDF3C(11, 0, 0, 0) == 0 && ids.values[1][4] != 7) {
        ovl_11_func_800E5C60(ids.values[0][3], 1, 0, 0);
        ids.values[0][3] = -1;
        ids.values[1][3] = -1;
    }
    if (ovl_11_func_800EDF3C(ids.values[1][0], 0, 0, 0) == 0) {
        ovl_11_func_800E5C60(ids.values[0][0], 1, 0, 0);
        ids.values[0][0] = -1;
        ids.values[1][0] = -1;
    }
    p = (s16 *)&ids;
    x = D_80124920;
    y = D_80124948;
    z = D_80124970;
    if (D_80129560[arg0] == -1) {
        for (i = 0; i < 10;) {
            if (*p != -1) {
                ovl_11_func_800E5ADC(*p, 0, *x, *y);
                ovl_11_func_800E5B84(*p, 0, *z, 1);
            }
            i++;
            p++;
            x++;
            y++;
            z++;
        }
        D_80129560[arg0]++;
    } else {
        for (i = 0; i < 10;) {
            if (*p != -1) {
                state = D_80129560[arg0];
                ovl_11_func_800F07EC(*p, *x, *y, &state);
            }
            i++;
            p++;
            x++;
            y++;
        }
        D_80129560[arg0] = state;
    }
    return 1;
}
