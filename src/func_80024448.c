#include "common.h"

void func_800248B0(s32 arg0, s16 arg1, s16 arg2);

void func_80024448(u16 arg0) {
    u16 temp;
    s32 quot;
    s32 rem;
    s32 add;

    temp = arg0;
    quot = temp / 14;
    rem = temp % 14;
    add = rem >= 5;
    if (rem >= 0xA) {
        add += 1;
    }
    if (rem >= 0xD) {
        add += 2;
    }
    rem += add;
    func_800248B0(gActiveRenderContext->field_D8 + 0x60,
                  (s16) (rem * 9 + 0x32),
                  (s16) (quot * 14 + 0x48));
}
