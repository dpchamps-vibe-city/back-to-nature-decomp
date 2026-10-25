#include "common.h"
#include "sound_commands.h"

s32 func_800226B0(void);
s32 ovl_11_func_800F8404(s32 arg0);
void ovl_11_func_800F6F7C(s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800F6F40(s32 arg0, s16 arg1, s16 arg2);

void ovl_11_func_800F84F0(s32 arg0) {
    union { s32 value; u16 halves[2]; } delta;
    s32 previous;
    s32 buttons;
    s32 pageButtons;
    s32 direction;
    s32 state;

    delta.halves[0] = 0;
    delta.halves[1] = 0;
    if (func_800226B0() != 0) {
        buttons = *(s32 *)gPad0Input;
        if (buttons & 0xF000) {
            if (buttons & 0x1000) {
                direction = delta.value;
            } else if (buttons & 0x4000) {
                direction = 1;
            } else if (buttons & 0x8000) {
                if (D_80126E44 == 9 || D_80126E44 == 13) {
                    delta.value = arg0 / 8 - 1;
                }
                direction = 2;
            } else {
                if (D_80126E44 == 12 || D_80126E44 == 16) {
                    delta.value = 1;
                }
                direction = 3;
            }
            if (D_80126E4E == 255) {
                D_80126E44 = D_80126E54[D_80126E44].links[direction][D_80071A1E];
                PlaySoundEffect(5);
            }
        }
        if (D_80126E4E == 255) {
            pageButtons = *(s32 *)gPad0Input;
            if (pageButtons & 5) {
                PlaySoundEffect(5);
                previous = delta.value - 1;
                delta.value = previous + arg0 / 8;
            } else if (pageButtons & 10) {
                PlaySoundEffect(5);
                delta.value++;
            }
        }
    }
    if (D_80126E4E == 255) {
        D_80126E4A = (u16)D_80126E4A + delta.value - ((D_80126E4A + delta.value) / (arg0 / 8)) * (arg0 / 8);
    }
    if (D_80126E48 != 255) {
        state = ovl_11_func_800F8404(D_80126E48);
        if (state >= 0) {
            if (state < 2 || (state == 2 && D_80126E4A == D_80126E4C)) {
                ovl_11_func_800F6F7C(gActiveRenderContext->field_D8 + 0x54,
                    D_80126E54[D_80126E48].x, D_80126E54[D_80126E48].y);
            }
        }
    }
    ovl_11_func_800F6F40(gActiveRenderContext->field_D8 + 0x50,
        D_80126E54[D_80126E44].x, D_80126E54[D_80126E44].y);
}
