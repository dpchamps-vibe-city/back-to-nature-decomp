#include "common.h"

s32 ovl_11_func_8011C298(s16 arg0, s16 arg1, s16 arg2, s16 arg3);

s32 ovl_11_func_80119414(void) {
    if (D_800719F8 & 0x40) {
        return ovl_11_func_8011C298(0x6D, 2, 0xD4, 1);
    }
    if (D_800719F8 & 0x80) {
        return ovl_11_func_8011C298(0x6E, 2, 0x248, 1);
    }
    return 1;
}
