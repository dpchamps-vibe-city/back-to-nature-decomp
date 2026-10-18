#include "common.h"

s32 ovl_11_func_800FFB08(void);
s32 func_800226B0(void);
void func_800226F0(void);
void ovl_11_func_800BCF28(void);

void ovl_11_func_800FFA64(void) {
    s32 temp_v0;

    if (D_801273DA == 0) {
        temp_v0 = ovl_11_func_800FFB08();
        D_8012CEF8 = temp_v0;
        if (temp_v0 != 0) {
            D_801273DA = 0xFE;
        }
    } else if (D_801273DA == 0xFE) {
        if (func_800226B0() != 0) {
            D_801273DA = 0xFF;
            func_800226F0();
            if (D_8012CEF8 == -1 || D_8012CEF8 == 3 ||
                D_8012CEF8 == 2 || D_8012CEF8 == 1) {
                ovl_11_func_800BCF28();
            }
        }
    }
}
