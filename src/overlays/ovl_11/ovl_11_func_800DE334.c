#include "common.h"

void ovl_11_func_800CEBC8(void);
void ovl_11_func_8010C95C(void);
void ovl_11_func_800CD45C(void);
void ovl_11_func_800C1BE0(void);

s32 ovl_11_func_800DE334(void) {
    ovl_11_func_800CEBC8();
    ovl_11_func_8010C95C();
    ovl_11_func_800CD45C();
    if (((struct struct_8006C838_time *) D_8006C838)->field_44C0 == 0xC) {
        ovl_11_func_800C1BE0();
    }
    if (((struct struct_8006C838_time *) D_8006C838)->field_44C0 == 0x12) {
        ovl_11_func_800C1BE0();
    }
    return 1;
}
