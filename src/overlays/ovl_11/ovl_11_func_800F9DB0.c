#include "common.h"

void func_8001316C(u32 arg0, u32 arg1, u32 arg2);
/* Overlay name for GetVal8005E5B4, the signed state-word getter. */
s32 GetVal8005E5B4(void);
void func_800226F0(void);
void ovl_11_func_800BCF28(void);
u32 func_80017A64(void);
void func_80017A48(u32 arg0);
void ovl_11_func_800F9E4C(void);

void ovl_11_func_800F9DB0(void) {
    u32 previous;

    if (D_80126F7C != 0) {
        func_8001316C(0x40000, 0x40000, 0x40000);
        if (D_80126F7C == 2) {
            if (GetVal8005E5B4() == 2) {
                func_800226F0();
                D_80126F7C = 0;
                ovl_11_func_800BCF28();
            }
        } else {
            previous = func_80017A64();
            func_80017A48(3);
            ovl_11_func_800F9E4C();
            func_80017A48(previous);
        }
    }
}
