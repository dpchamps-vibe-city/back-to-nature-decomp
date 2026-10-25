#include "common.h"
#include "game_types.h"

s32 ovl_11_func_8011C6CC(s16 *arg0, s16 *arg1, s16 *arg2, Ovl11D8012D7B8 arg3);

s32 ovl_11_func_801194DC(void) {
    s16 start[2];
    s16 next[2];
    s16 cancel[2];
    Ovl11D8012D7B8 params;

    start[0] = 2;
    start[1] = 0xC0;
    next[0] = 2;
    next[1] = 0xBB;
    cancel[0] = 2;
    cancel[1] = 0xC2;
    params.unk4 = 2;
    params.unk6 = 0xBE;
    params.unk8 = 2;
    params.unkA = 0xBF;
    params.unkC = 2;
    params.unkE = 0xBA;
    params.unk10 = 4;
    params.unk0 = gOvl11PrimarySelectionRecord;
    if (((Ovl11FB908State *)D_8006C838)->field_44D4 == 0) {
        if (((struct struct_8006C838_button *)D_8006C838)->field_44BA != 0) {
            return 1;
        }
    } else {
        params.unk4 = 2;
        params.unk6 = 0x275;
    }
    return ovl_11_func_8011C6CC(start, next, cancel, params);
}
