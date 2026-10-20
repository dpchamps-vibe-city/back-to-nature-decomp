#include "common.h"
#include "game_types.h"

s32 ovl_11_func_8011C6CC(s16 *arg0, s16 *arg1, s16 *arg2, Ovl11D8012D7B8 arg3);

s32 ovl_11_func_8011977C(void) {
    s16 start[2];
    s16 next[2];
    s16 cancel[2];
    Ovl11D8012D7B8 params;

    start[0] = 2;
    start[1] = 0x16E;
    next[0] = 2;
    next[1] = 0x170;
    cancel[0] = 2;
    cancel[1] = 0x171;
    params.unk4 = 2;
    params.unk6 = 0x164;
    params.unk8 = 2;
    params.unkA = 0x16D;
    params.unkC = 2;
    params.unkE = 0x16F;
    params.unk10 = 4;
    params.unk0 = D_8012D548;
    return ovl_11_func_8011C6CC(start, next, cancel, params);
}
