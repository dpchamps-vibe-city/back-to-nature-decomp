#include "common.h"
#include "psyq/stdio.h"
#include "psyq/libapi.h"

s32 ovl_31_func_800B8600(void) {
    s32 var_s0;

    sprintf(D_800B8870, D_800B7F20, 0, 0, D_800B7F2C);
    sprintf(D_800B8890, D_800B7F20, 0, 0, D_800B7F40);
    var_s0 = 0;
    while (rename(D_800B8870, D_800B8890) != 1) {
        if (var_s0 < 3) {
            var_s0++;
        } else {
            break;
        }
    }
    sprintf(D_800B8870, D_800B7F20, 0, 0, Ovl31_D_800B7F54);
    sprintf(D_800B8890, D_800B7F20, 0, 0, D_800B7F68);
    var_s0 = 0;
    while (rename(D_800B8870, D_800B8890) != 1) {
        if (var_s0 < 3) {
            var_s0++;
        } else {
            break;
        }
    }
    sprintf(D_800B8870, D_800B7F20, 0, 0, D_800B7F7C);
    sprintf(D_800B8890, D_800B7F20, 0, 0, D_800B7F90);
    var_s0 = 0;
    while (rename(D_800B8870, D_800B8890) != 1) {
        if (var_s0 < 3) {
            var_s0++;
        } else {
            break;
        }
    }
    sprintf(D_800B8870, D_800B7F20, 0, 0, D_800B7FA4);
    sprintf(D_800B8890, D_800B7F20, 0, 0, D_800B7FB8);
    var_s0 = 0;
    while (rename(D_800B8870, D_800B8890) != 1) {
        if (var_s0 < 3) {
            var_s0++;
        } else {
            break;
        }
    }
    return 1;
}
