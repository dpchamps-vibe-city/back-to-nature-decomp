#include "common.h"
#include "game_types.h"
#include "ovl11_record_position_tables.h"

void ovl_11_func_800D12B8(Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1, s32 arg2);

s32 ovl_11_func_8010E644(Ovl11E644Object *arg0) {
    s16 var_a0;
    s16 var_v1;

    ovl_11_func_800D12B8((Recon_ovl_11_func_800D2594_A0View *) arg0, Ovl11GetRecordMovePeriod((s32) arg0), 0x70);
    var_a0 = 0xF;
    if (arg0->unkB6 == 0) {
        var_a0 = 0;
        var_v1 = 1;
    } else {
        var_v1 = 6;
    }
    if ((arg0->unk48 == 0) && (arg0->unk4C == 0) && (arg0->unk50 == 0)) {
        arg0->unk24 = var_a0;
    } else {
        arg0->unk24 = var_v1;
    }
    arg0->unk34 &= ~0x800;
    return 1;
}
