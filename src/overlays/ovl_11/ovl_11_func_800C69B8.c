#include "common.h"
#include "game_types.h"

void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, u16 arg1, s32 arg2, s32 arg3);
s32 func_8001FABC(s16 arg0);

s32 ovl_11_func_800C69B8(Ovl11C7010Object *arg0) {
    if ((D_8006C858[arg0->itemIndex].u0.flags & 0x1200) == 0x1200) {
        ovl_11_func_800C70CC(arg0, 3, 5, -2);
        func_8001FABC(0x4A);
        return 1;
    }
    return 0;
}
