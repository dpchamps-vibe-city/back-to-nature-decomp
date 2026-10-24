#include "common.h"
#include "psyq/stddef.h"

/* Overlay import of matched u32 Rand(s32) at 0x80012A34. */
u32 Rand(s32 arg0);
s32 func_8001AD6C(s32 arg0);
void func_8001ADD8(u32 *arg0, u16 arg1, s16 arg2, s16 arg3);
void ovl_28_func_800B8E9C(u_long *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s16 arg9, u8 arg10);
/* Matched callee takes no arguments: $a0 is first defined at 800B911C. */
void ovl_28_func_800B9074(void);
void ovl_28_func_800B92F0(s16 arg0, s16 arg1);
void ovl_28_func_800B9328(s16 arg0, s16 arg1);
s32 ovl_28_func_800B935C(void);

void ovl_28_func_800B86D8(void) {
    u32 *ot;
    s32 column;

    ot = (u32 *)D_8005E3C0->field_D8 + 4;
    if (D_800B93C0 == 1) {
        switch (D_800B93C4) {
        case -1:
            if (D_800B93B8 == 1) {
                column = D_800B93CE;
                D_800B93CE = (u16)D_800B93CE + 1;
                D_800B93C6 = D_800B93D0[D_800B93CC][column];
                D_800B93C8 = Rand((u16)(*(u16 *)&HWD0 - 80));
                D_800B93CA = 144;
                D_800B93C4 = -2;
            }
            break;
        case -2:
            if (func_8001AD6C((u16)D_800B93C6) == 1) {
                ovl_28_func_800B92F0(D_800B93C8, D_800B93CA);
                D_800B93C4 = -3;
            }
            break;
        case -3:
            ovl_28_func_800B9074();
            func_8001ADD8(ot, (u16)D_800B93C6, D_800B93C8, D_800B93CA);
            if (ovl_28_func_800B935C() != 0) {
                D_800B93C4 = 60;
            }
            break;
        case 0:
            ovl_28_func_800B9074();
            func_8001ADD8(ot, (u16)D_800B93C6, D_800B93C8, D_800B93CA);
            if (ovl_28_func_800B935C() != 0) {
                D_800B93C4 = -1;
            }
            break;
        default:
            func_8001ADD8(ot, (u16)D_800B93C6, D_800B93C8, D_800B93CA);
            ovl_28_func_800B8E9C((u_long *)D_8005E3C0->field_D8 + 3,
                D_800B93C8, D_800B93CA, 80, 80,
                0x4B6496, 0x4B6496, 0x4B6496, 0x4B6496, 1, 2);
            if (--D_800B93C4 == 0) {
                ovl_28_func_800B9328(D_800B93C8, D_800B93CA);
            }
            break;
        }
    }
}
