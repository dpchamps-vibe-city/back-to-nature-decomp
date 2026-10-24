#include "common.h"
#include "game_types.h"

s32 ovl_11_func_8010B898(u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);
void func_80015840();
void func_8001585C();
/* SetVal8005E43C in the executable: writes its s32 argument to D_8005E43C. */
void SetVal8005E43C(s32 arg0);
void ovl_11_func_800CFE40(u8 *arg0);

void ovl_11_func_8010B0E8(u8 *arg0) {
    u16 animation;
    u16 subAnimation;

    if (!(D_8006C844 & 0x800000) && *(s32 *)(arg0 + 0x8C) != 0) {
        ovl_11_func_8010B898(*(u16 *)(arg0 + 0x24), *(u16 *)(arg0 + 0x22),
                            &animation, &subAnimation);
        if (animation != arg0[0x7C]) {
            func_80015840((ObjectState *)(arg0 + 0x78), (u8)animation);
        }
        if (subAnimation != 0 && subAnimation != arg0[0x7D]) {
            func_8001585C((ObjectState *)(arg0 + 0x78), (u8)subAnimation);
        }
        SetVal8005E43C(0);
        ovl_11_func_800CFE40(arg0);
    }
}
