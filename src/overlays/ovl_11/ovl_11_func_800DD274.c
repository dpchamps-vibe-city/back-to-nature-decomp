#include "common.h"

s32 ovl_11_func_800DD274(void) {
    s16 temp_a1;
    s16 temp_a1_2;
    s32 var_a1;

    var_a1 = 0;
    if (((struct Ovl11MessageState *)D_8006C838)->variant50 == 0) {
        temp_a1 = D_80123E38[((struct Ovl11MessageState *)D_8006C838)->variant50];
        if (((struct Ovl11MessageState *)D_8006C838)->variant70 == 0) {
            var_a1 = temp_a1 + D_80123E48;
        } else {
            var_a1 = temp_a1 + D_80123E4A;
        }
    }
    if (((struct Ovl11MessageState *)D_8006C838)->variant50 == 1) {
        temp_a1_2 = D_80123E38[((struct Ovl11MessageState *)D_8006C838)->variant50];
        if (((struct Ovl11MessageState *)D_8006C838)->variant60 == 0) {
            var_a1 = temp_a1_2 + D_80123E40;
        } else {
            var_a1 = temp_a1_2 + D_80123E42;
        }
    }
    if (((struct Ovl11MessageState *)D_8006C838)->variant50 == 2) {
        var_a1 = D_80123E38[((struct Ovl11MessageState *)D_8006C838)->variant50];
        var_a1 += D_80123E50[((struct Ovl11MessageState *)D_8006C838)->variant80];
    }
    return var_a1 + ((struct Ovl11MessageState *)D_8006C838)->base;
}
