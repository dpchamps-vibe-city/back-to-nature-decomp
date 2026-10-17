#include "common.h"

void ovl_11_func_800E4568(void);
void ovl_11_func_800E4428(void);
void ovl_11_func_800E54C8(void);
void ovl_11_func_800E549C(void);
void ovl_11_func_800E5230(void);
s32 ovl_11_func_800DBF60(void);
void ovl_11_func_800E537C(void);
void ovl_11_func_800E5294();

s32 ovl_11_func_800E4280(void) {
    ((struct Ovl11E4280State *)D_8006C838)->flags_C &= ~0x400000;
    ovl_11_func_800E4568();
    ovl_11_func_800E4428();
    ovl_11_func_800E54C8();
    ovl_11_func_800E549C();
    ((struct Ovl11E4280State *)D_8006C838)->field_5214 = 0;
    ((struct Ovl11E4280State *)D_8006C838)->field_448C = 0;
    ((struct Ovl11E4280State *)D_8006C838)->flags_4450 &= ~0x10;
    ovl_11_func_800E5230();
    if (ovl_11_func_800DBF60() == 1) {
        ovl_11_func_800E5294();
    } else {
        ovl_11_func_800E537C();
    }
    ((struct Ovl11E4280State *)D_8006C838)->selector = 0;
    return 1;
}
