#include "common.h"

s32 ovl_11_func_800C0A28(void);
void ovl_11_func_800C0688(void);
s32 ovl_11_func_800E9B68(s16 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800DE4C0(void);
void ovl_11_func_800BFE3C(void);
void ovl_11_func_800C19B0();
void ovl_11_func_800F227C(s16 arg0);

void ovl_11_func_800C06B0(void) {
    s32 var_s0;
    struct Ovl11ClockAnchor *anchor;

    if (D_80128B50 != 0) {
        D_80128B54 = 0;
        if (ovl_11_func_800C0A28() == 0) {
            anchor = (struct Ovl11ClockAnchor *)&D_8006C904;
            while (D_80128B58 > 0) {
                anchor->counter = D_80122F78;
                D_80128B54 = 1;
                ovl_11_func_800C0688();
                D_80128B54 = 0;
                D_80128B58 -= 1;
                if (ovl_11_func_800C0A28() != 0) {
                    return;
                }
            }
            anchor->counter = 0;
            D_80128B50 = 0;
            D_80128B54 = 1;
            if (((struct Ovl11StateInitView *)D_8006C838)->field_44C0 < 6) {
                for (var_s0 = 0; var_s0 < 0x25; var_s0++) {
                    ovl_11_func_800E9B68((s16)var_s0, 0, 0, 0);
                }
            } else {
                ovl_11_func_800F227C(((struct Ovl11StateInitView *)D_8006C838)->field_44CA);
                ovl_11_func_800C19B0();
            }
            if (D_80128B5C == 1) {
                ovl_11_func_800DE4C0();
                D_80128B5C = 0;
            }
            ovl_11_func_800BFE3C();
        }
    }
}
