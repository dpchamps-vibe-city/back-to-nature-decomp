#include "common.h"

void ovl_27_func_800B7F2C(void);
void func_80021DA8(void);
u32 Rand(s32 arg0);
void ovl_27_func_800BA914(s32 arg0);
void ovl_27_func_800BAA34(void);
void ovl_27_func_800BABD4(void);
void ovl_27_func_800BAC14(void);
void func_800132F0(s32 arg0, s32 arg1, s32 arg2);
void func_800226F0(void);
void ovl_27_func_800BA1CC(void);

void ovl_27_func_800B7FE8(void) {
    u8 *base;
    s32 flags;
    s32 sel;

    ovl_27_func_800B7F2C();
    base = (u8 *)&D_8006C838;
    flags = *(s32 *)(base + 0xC);
    D_800C4A28 = flags;
    *(s32 *)(base + 0xC) = flags & 0xFFFF7FFF;
    if (!(flags & 0x4000)) {
        sel = *(s32 *)(base + 0x18);
        func_80021DA8();
        *(s32 *)(base + 0x18) = sel;
        ovl_27_func_800BA914(Rand(3));
        ovl_27_func_800BAA34();
        ovl_27_func_800BABD4();
        ovl_27_func_800BAC14();
    }
    func_800132F0(0, 0, 2);
    func_800226F0();
    ovl_27_func_800BA1CC();
    *(s32 *)(base + 0xC) &= -0x1001;
    *(s32 *)(base + 0x4488) += 1;
}
