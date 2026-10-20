#include "common.h"

void ovl_11_func_800CD0C0(s32 *arg0);
/* This call supplies four zero arguments; the callee only reads the first two. */
s32 ovl_11_func_800E9284();
s32 ovl_11_func_800CB4E0(void);

s32 ovl_11_func_800E8E78(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 *position = D_80071B00;

    switch (arg0) {
    case 0:
        position[1] = arg1;
        position[0] = arg2;
        position[2] = arg3;
        break;
    case 1:
        position[1] = D_80129560[arg1];
        position[0] = D_80129560[arg2];
        position[2] = D_80129560[arg3];
        break;
    case 2:
        ovl_11_func_800CD0C0(position);
        ovl_11_func_800E9284(0, 0, 0, 0);
        break;
    }
    ovl_11_func_800CB4E0();
    ((Ovl11CBE2CState *)D_8006C838)->collision =
        ((Ovl11CollisionWorkView *)&D_8007AFF0)->collision;
    return 1;
}
