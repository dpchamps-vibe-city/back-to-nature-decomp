#include "common.h"

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800FDFF4(u16 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800FE068(u16 arg0, s32 arg1, s32 arg2);
s32 ovl_11_func_800FDE98(u16 arg0);
void ovl_11_func_800FE14C(s32 input_level, u32 input_value, s32 input_x, s32 input_y);
void ovl_11_func_800FE3A0(s32 input_level, u32 input_value, s32 input_x, s32 input_y);

void ovl_11_func_800FDD74(void) {
    s32 i;
    s32 value;

    func_80017B3C(gActiveRenderContext->field_D8 + 0x54,
                 (s32)(D_80053780 + D_80054BBC[0]), 0xBA, 0x1A);
    for (i = 0; i < 5; i++) {
        ovl_11_func_800FDFF4(i, 0x28, (s16)(0x30 + i * 0x22));
        ovl_11_func_800FE068(((struct Ovl11FDD74State *)D_8006C838)->entries[i], 0x48, (s16)(0x44 + i * 0x22));
        value = ovl_11_func_800FDE98(i);
        ovl_11_func_800FE14C(value, ((struct Ovl11FDD74State *)D_8006C838)->entries[i], 0x50, (s16)(0x38 + i * 0x22));
        ovl_11_func_800FE3A0(value, ((struct Ovl11FDD74State *)D_8006C838)->entries[i], 0xD7, (s16)(0x3C + i * 0x22));
    }
}
