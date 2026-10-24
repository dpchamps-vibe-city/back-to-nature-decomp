#include "common.h"

extern s16 D_800BFE46;

s32 GetVal8005E5B4(void);

s32 func_8002261C(s32 arg0, s32 arg1);

void ovl_25_func_800BA818(void) {
    s32 var_v1;

    func_8002261C(4, 0x31);
    var_v1 = GetVal8005E5B4();
    if (var_v1 == 2) {
        D_800BFE46 = var_v1;
    }
}
