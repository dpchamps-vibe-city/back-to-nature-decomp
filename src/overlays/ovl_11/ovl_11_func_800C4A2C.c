#include "common.h"
#include "game_types.h"
#include "ovl11_cell_grid_lookup.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_cell_actions.h"

s32 ovl_11_func_800C9D64(void);
s32 ovl_11_func_800D806C(s16 arg0, s16 arg1, s16 arg2, s16 arg3);

void ovl_11_func_800C4A2C(Ovl11C4A2CObject *arg0) {
    Ovl11CE034Position sp18;
    u16 sp28;
    u16 sp2A;
    s16 sp2C;
    s16 sp2E;
    s32 temp_s3;
    u32 var_s1;

    temp_s3 = ovl_11_func_800C9D64();
    sp18.unk0 = arg0->unk100;
    sp18.unk4 = arg0->unk104;
    sp18.unk8 = arg0->unk108;
    if (temp_s3 != 2) {
        D_80123234[temp_s3](&sp18, &sp28, &sp2A);
        var_s1 = 0;
        do {
            if (Ovl11FindCellGridTag(&arg0->unk84, var_s1, arg0->unk38, &sp2C, &sp2E, 0) != 0) {
                sp2C += sp28;
                sp2E += sp2A;
                if (ovl_11_func_800D806C(sp2C, sp2E, 1, (s16) temp_s3) == 0) {
                    Ovl11TryInstallCellItem((s16) temp_s3, sp2C, sp2E, arg0->unk84);
                }
            }
            var_s1 += 1;
        } while (var_s1 < 9U);
    }
}
