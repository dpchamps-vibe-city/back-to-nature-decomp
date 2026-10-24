#include "common.h"
#include "game_types.h"

void func_80015114(u32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s16 arg6);
u32 func_80017A64(void);
void func_80017A48(u32 arg0);
/* Overlay import for the executable's GetVal8005E5B4. */
s32 GetVal8005E5B4(void);
void func_800226F0(void);
void ovl_11_func_800BCF28(void);
void ovl_11_func_800FAC0C(void);
void ovl_11_func_800CCF58(Ovl11C7010Object *arg0);

void ovl_11_func_800FAB18(void) {
    u32 previous;
    u32 *flags;

    if (D_80126FE0 != 0) {
        func_80015114((u32 *)(D_8005E3C0->field_D8 + 0x68), 0, 0,
            (s16)HWD0, (s16)VWD0, 0, 1);
        if (D_80126FE0 == 3) {
            if (GetVal8005E5B4() == 2) {
                func_800226F0();
                D_80126FE0 = 0;
                ovl_11_func_800BCF28();
                flags = (u32 *)D_8006C838;
                flags[3] |= 0x40000;
                flags[0x5234 / 4] |= 0x2000;
            }
        } else {
            previous = func_80017A64();
            func_80017A48(3);
            ovl_11_func_800FAC0C();
            func_80017A48(previous);
            ovl_11_func_800CCF58((Ovl11C7010Object *)&D_80071A00);
        }
    }
}
