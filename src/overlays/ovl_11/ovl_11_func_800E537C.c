#include "common.h"

void func_8001AF70(u16 arg0, u16 arg1);
s32 ovl_11_func_800E880C(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
/* The original calls explicitly supply zero in both trailing argument slots. */
s32 ovl_11_func_800E63C8(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E6EAC(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
void ovl_11_func_800F4CEC(void);
void ovl_11_func_8010941C(s32 arg0);

void ovl_11_func_800E537C(void) {
    func_8001AF70(3, 0);
    ovl_11_func_800E880C(-2, 0, 0, 0);
    ovl_11_func_800E63C8(1, 0, 0, 0);
    ((struct Ovl11ResetState *) &D_8006C838)->field_51EA = -1;
    ((struct Ovl11ResetState *) &D_8006C838)->field_51F0 = 0;
    ((struct Ovl11ResetState *) &D_8006C838)->field_5234 &= ~0x100000;
    ((struct Ovl11ResetState *) &D_8006C838)->field_4450 &= ~2;
    ((struct Ovl11ResetState *) &D_8006C838)->field_51FE &= ~8;
    if ((ovl_11_func_800E6EAC(1, 0, 0, 0) == 0) &&
        (((struct Ovl11ResetState *) &D_8006C838)->field_5204 != 0xC)) {
        ((struct Ovl11ResetState *) &D_8006C838)->field_5234 =
            (((struct Ovl11ResetState *) &D_8006C838)->field_5234 & ~0x4000) | 0x8000;
    }
    if (*((struct Ovl11ResetState *) &D_8006C838)->field_DDD4 == 0x36) {
        ovl_11_func_800F4CEC();
    }
    if (((struct Ovl11ResetState *) &D_8006C838)->field_91DC == 0x36) {
        ovl_11_func_8010941C(0);
    }
}
