#include "common.h"
#include "screen_primitives.h"

/* TU-owned gp-rel global: the target's lhu/sw %gp_rel(D_8005E338) proves this
 * TU declares it; tentative definition merges via -fcommon with
 * func_8002301C / func_80023030 / func_80023288 / func_800233B4 siblings. */
u16 D_8005E338;

/* Callee prototypes as this caller TU saw them. */
void func_80023288(void);

s32 func_80023060(void) {
    if (D_8005E338 != 0) {
        DrawScreenTileRect((u32 *)(gActiveRenderContext->field_D8 + 0x6C), 0, 0, D_800A3FB0, D_800A3FB4, 0, 1);
        func_80023288();
        switch (D_8005E338) {
        case 4:
            D_8005E338 = 0;
            return -1;
        case 5:
            D_8005E338 = 0;
            return 1;
        default:
            return 2;
        }
    } else {
        return 0;
    }
}
