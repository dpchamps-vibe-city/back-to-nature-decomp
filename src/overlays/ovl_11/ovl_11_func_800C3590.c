#include "common.h"
#include "game_types.h"

void ovl_11_func_800C36D0(Ovl11AnimationObject *arg0);

void ovl_11_func_800C3590(Ovl11AnimationObject *arg0) {
    if (*(s16 *) ((u8 *) arg0 + 0x2E) ==
            ((struct Ovl11D2764StateView *) &D_8007AFF0)->field_25476 &&
        !(D_8006C844 & 0x800000)) {
        ovl_11_func_800C36D0(arg0);
    }
}
