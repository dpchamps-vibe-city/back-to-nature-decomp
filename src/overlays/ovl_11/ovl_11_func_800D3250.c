#include "common.h"

s32 ovl_11_func_800C0824(void);

/* The callee masks its only read incoming argument to u16. */
s32 ovl_11_func_800D3D60(u16 arg0);

s32 ovl_11_func_800D3250(u16 *arg0) {
    s32 second;
    s32 index;
    u8 *state;
    struct Ovl11D3250SecondView *base;
    u16 *value;
    s32 current;

    if (*arg0 != 0x15B) {
        return -1;
    }
    if (ovl_11_func_800C0824() == 1) {
        state = (u8 *)D_8006C838;
        if (*(s32 *)(state + 0x44F8) & 0x01000000) {
            if (*(s16 *)(state + 0x44C0) >= 7) {
                second = 0;
                index = ovl_11_func_800D3D60(0);
                if (index < 0) {
                    second = 1;
                    index = ovl_11_func_800D3D60(1);
                }
                if (second == 0) {
                    if (index > 0) {
                        s32 scale;
                        value = (u16 *)((u8 *)&D_80074A0A + index * 0xB8);
                        scale = (s16)D_80074A0A.scale;
                        current = *value - 10;
                        current += scale / 50;
                        *value = current;
                    }
                } else if (index > 0) {
                    s32 scale;
                    base = (struct Ovl11D3250SecondView *)(state + 0x7ACA);
                    value = (u16 *)((u8 *)base + index * 0xB4);
                    scale = (s16)base->scale;
                    current = *value - 10;
                    current += scale / 50;
                    *value = current;
                }
            }
        }
    }
    return 0;
}
