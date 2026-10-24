#include "common.h"
#include "game_types.h"

s32 ovl_11_func_80109550(u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);
void func_80015840();
void func_8001585C();
/* Engine setter at 0x800158D8, named SetVal8005E43C in the executable. */
void SetVal8005E43C(s32 arg0);
void ovl_11_func_800D0198(u8 *arg0, s32 arg1, s32 arg2);

void ovl_11_func_80108BE8(u8 *arg0) {
    u16 animation;
    u16 subAnimation;

    if (!(D_8006C844 & 0x800000)) {
        ovl_11_func_80109550(*(u16 *)(arg0 + 0x24), *(u16 *)(arg0 + 0x22),
                            &animation, &subAnimation);
        if (animation != arg0[0x7C]) {
            func_80015840((ObjectState *)(arg0 + 0x78), (u8)animation);
        }
        if (subAnimation != 0 && subAnimation != arg0[0x7D]) {
            func_8001585C((ObjectState *)(arg0 + 0x78), (u8)subAnimation);
        }
        SetVal8005E43C(0);
        ovl_11_func_800D0198(arg0, (s32)(arg0 + 0xE0 + D_8005E3A4 * 8), 0x2800);
    }
}
