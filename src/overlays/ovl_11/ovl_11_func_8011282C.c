#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


s32 ovl_11_func_800E6EE0 (s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800EEC1C (s16 id, s16 which, s16 last, s32 amount);

void ovl_11_func_8011282C(void) {
    D_800719F8 &= 0xFF9FFFFF;
    if (ovl_11_func_800E6EE0(0x1F, 0, 2, 0) == 0) {
        D_800719F8 |= 0x200000;
    }
    if (ovl_11_func_800E6EE0(0x20, 0, 2, 0) == 0) {
        D_800719F8 |= 0x400000;
    }
}
