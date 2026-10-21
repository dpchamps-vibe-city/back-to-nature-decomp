#include "common.h"
#include "game_types.h"

s32 func_8001AF44(u32 arg0);
s32 func_80013394(void);
void ovl_11_func_800C2B04(Ovl11C3B78Arg0 *arg0);
void ovl_11_func_800C2CE8(Ovl11C3B78Arg0 *arg0, s32 arg1);

void ovl_11_func_800C2F74(Ovl11C3B78Arg0 *arg0) {
    u16 temp_v1;
    u16 temp_v1_2;

    temp_v1 = *(u16 *) ((u8 *) arg0 + 0x1E);
    if (temp_v1 & 0x8000) {
        (*(u16 *) ((u8 *) arg0 + 0x1E)) = (u16) (temp_v1 & 0x7FFF);
    }
    if (!(D_8006C844 & 0x08000000)) {
        if (func_8001AF44(3U) == 0) {
            ovl_11_func_800C2B04(arg0);
        }
        if (func_80013394() != 0) {
            temp_v1_2 = *(u16 *) ((u8 *) arg0 + 0x1E);
            if (temp_v1_2 & 0x100) {
                if (!(temp_v1_2 & 0x20)) {
                    if (func_8001AF44(3U) == 0) {
                        ovl_11_func_800C2CE8(arg0, 1);
                    }
                }
            }
        }
    }
}
