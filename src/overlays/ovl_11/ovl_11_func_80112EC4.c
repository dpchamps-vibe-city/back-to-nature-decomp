#include "common.h"

void func_8001AF70(u16 arg0, u16 arg1);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800CB4E0(void);

s32 ovl_11_func_80112EC4(s32 arg0, s32 arg1, s16 arg2, s16 arg3) {
    if (((Ovl11GridState *)D_8012D110)->grid[arg1][arg2][arg3] == 2) {
        ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x18000000;
        ((Ovl11GridState *)D_8012D110)->flags |= arg0 != 0 ? 0x8000 : 0x4000;
        func_8001AF70(3, 1);
        if (arg0 == 0) {
            ovl_11_func_80118B84(D_8005E3C0->field_D8 + 0x3C, -1, 0);
        }
        ovl_11_func_800CB4E0();
        return 1;
    }
    return 0;
}
