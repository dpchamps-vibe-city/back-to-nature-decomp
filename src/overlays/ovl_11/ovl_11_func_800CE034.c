#include "common.h"
#include "game_types.h"

void ovl_11_func_800CE034(Ovl11CE034Object *arg0, Ovl11CE034Position *arg1, s32 arg2) {
    u16 *var_t0;
    s32 var_a3;
    s32 var_a0;

    var_t0 = (u16 *)(gOvl11AreaGridCellOffsets + arg0->unk38 * 4);
    if (arg2 == 2) {
        var_t0 = (u16 *)(gOvl11AreaGridCellOffsets + arg0->unk38 * 4 + 0x10);
    }
    var_a3 = arg0->unk100;
    arg1->unk0 = var_a3;
    arg1->unk4 = arg0->unk104;
    var_a0 = arg0->unk108;
    arg1->unk8 = var_a0;
    if (arg2 == 4) {
        arg1->unk0 = var_a3 + (s16)var_t0[0] / 2;
        arg1->unk8 = var_a0 + (s16)var_t0[1] / 2;
        return;
    }
    arg1->unk0 = var_a3 + (s16)var_t0[0];
    arg1->unk8 = var_a0 + (s16)var_t0[1];
}
