#include "common.h"

s32 func_8001AF44(u32 arg0);
void func_8001AF70(u16 arg0, u16 arg1);
void ovl_11_func_800BDA70(void);
void ovl_11_func_800C1BE0(void);
void ovl_11_func_800BD6A8(s16 arg0, s16 arg1);
void ovl_11_func_800C19B0(void);

void ovl_11_func_800C3D70(void) {
    s16 temp_a1;

    if (!((u16)((struct Ovl11SpriteWork *)&D_8007AFF0)->header[4] & 0x1000) && (temp_a1 = ((struct Ovl11SpriteWork *)&D_8007AFF0)->header[1], (D_80125F24[temp_a1] != 0x1000))) {
        if (temp_a1 == 1) {
            if ((D_80070D02 == 0x438) && (func_8001AF44(0x40U) == 0)) {
                ovl_11_func_800BDA70();
                func_8001AF70(0x40U, 1U);
            }
        } else {
            if (((struct Ovl11CCCC0State *)D_8006C838)->angle == 0x2D0) {
                ovl_11_func_800C1BE0();
                if (func_8001AF44(0x3FU) == 0) {
                    if (!(((struct Ovl11CCCC0State *)D_8006C838)->flags & 0x10)) {
                        ovl_11_func_800BD6A8(0xE, 1);
                        ovl_11_func_800C19B0();
                    }
                    func_8001AF70(0x3FU, 1U);
                }
            }
            if (((struct Ovl11CCCC0State *)D_8006C838)->angle == 0x438) {
                ovl_11_func_800C1BE0();
                if (func_8001AF44(0x40U) == 0) {
                    if (!(((struct Ovl11CCCC0State *)D_8006C838)->flags & 0x10)) {
                        ovl_11_func_800BD6A8(0xE, 1);
                        ovl_11_func_800C19B0();
                    }
                    func_8001AF70(0x40U, 1U);
                }
            }
        }
    }
}
