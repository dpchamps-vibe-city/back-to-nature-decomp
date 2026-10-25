#include "common.h"

s32 func_800226B0(void);
s32 func_8001FABC(s16 arg0);
void ovl_11_func_800FA4F4(s32 arg0, s16 arg1, s16 arg2);

void ovl_11_func_800F9FF4(void) {
    s32 buttons;
    s32 weekday;
    s16 date;

    if ((D_80126F80 == 0xFF) && (func_800226B0() != 0)) {
        if (*(s32 *) D_8005E3A8 & 0xF000) {
            func_8001FABC(5);
            buttons = *(s32 *) D_8005E3A8;
            if (buttons & 0x1000) {
                D_80126F84 -= 7;
                if (D_80126F84 < 0) {
                    D_80126F84 += 28;
                    if (D_80126F84 < 23) {
                        D_80126F84 += 7;
                    }
                }
            } else if (buttons & 0x4000) {
                D_80126F84 += 7;
                if (D_80126F84 >= 30) {
                    D_80126F84 -= 28;
                    if (D_80126F84 >= 7) {
                        D_80126F84 -= 7;
                    }
                }
            } else if (buttons & 0x8000) {
                --D_80126F84;
                if (D_80126F84 < 0) {
                    D_80126F84 += 30;
                }
            } else {
                ++D_80126F84;
                if (D_80126F84 >= 30) {
                    D_80126F84 -= 30;
                }
            }
        }
    }
    date = ((struct Ovl11MenuDateView *) D_8006C838)->field_44BC;
    weekday = (s16) (date % 7);
    weekday -= 7;
    ovl_11_func_800FA4F4(gActiveRenderContext->field_D8 + 0x50, (s16) D_80126F84,
        (s16) ((((struct Ovl11MenuDateView *) D_8006C838)->field_44BE - weekday) % 7));
}
