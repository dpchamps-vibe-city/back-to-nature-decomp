#include "common.h"
#include "screen_primitives.h"

/* TU-owned gp-rel global: the target's lhu/sw %gp_rel(D_8005E344) proves this
 * TU declares it; tentative definition merges via -fcommon with
 * func_80023100.c / func_800239F8.c / func_800239F8.c siblings. */
u16 D_8005E344;

/* Callee prototypes as this caller TU saw them. */
void func_800239F8(void);

s32 func_800231E8(void) {
    if (D_8005E344 != 0) {
        DrawScreenTileRect((u32 *)(gActiveRenderContext->field_D8 + 0x6C), 0, 0, D_800A3FB0, D_800A3FB4, 0, 1);
        func_800239F8();
        switch (D_8005E344) {
        case 3:
            D_8005E344 = 0;
            return -1;
        case 4:
            D_8005E344 = 0;
            return 1;
        default:
            return 2;
        }
    } else {
        return 0;
    }
}
