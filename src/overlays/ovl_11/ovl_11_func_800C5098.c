#include "common.h"
#include "game_types.h"
#include "ovl11_cell_grid_lookup.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_cell_actions.h"

s32 ovl_11_func_800D806C(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
void ovl_11_func_800CD5BC(Ovl11CD5BCObject *arg0, s32 arg1);

s32 ovl_11_func_800C5098(Ovl11CD5BCObject *arg0) {
    s16 sp18;
    s16 sp1A;

    if (D_80128CB4 <= D_80128CB8) {
        if (Ovl11FindCellGridTag(&arg0->value, (u32) D_80128CB4, *(s16 *) ((u8 *) arg0 + 0x38), &sp18, &sp1A, D_80128CC8) != 0) {
            sp18 += D_80128CC0;
            sp1A += D_80128CC2;
            if (ovl_11_func_800D806C(sp18, sp1A, 0, D_80128CC4) != 1) {
                switch (D_80128CC6) {
                case 0:
                    if (Ovl11TryResetCellAndSpawnEffect(D_80128CC4, sp18, sp1A) == 1) {
                        D_80128CBC = 0x96;
                    } else {
                        D_80128CB4 = D_80128CB8 + 1;
                    }
                    break;
                case 1:
                    if (Ovl11TryPromoteCellPendingKind(D_80128CC4, sp18, sp1A, 0) == 1) {
                        D_80128CBC = 0x96;
                    }
                    break;
                case 2:
                    if (Ovl11TryApplyCellActionEffect(D_80128CC4, sp18, sp1A) == 1) {
                        D_80128CBC = 0x96;
                    }
                    break;
                }
            }
        }
    } else {
        ovl_11_func_800CD5BC(arg0, D_80128CBC);
        return 1;
    }
    D_80128CB4 += 1;
    return 0;
}
