#include "common.h"
#include "game_types.h"

void ovl_19_func_800BA0A0(Recon_ovl_19_func_800BA0A0_A0View *arg0) {
    s16 temp_v0;
    s16 temp_v1;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;

    temp_v0 = arg0->unk2 - 3;
    var_a1 = 0;
    switch (temp_v0) {
    case 2:
        var_a1 = D_800BF4C0[0xB8] * D_800BF4C0[0xCB] / 10;
        break;
    case 4:
        var_a1 = D_800BF4C0[0xB8] * D_800BF4C0[0xCC] * D_800BF4C0[0xCD] / 100;
        break;
    case 5:
        var_a1 = D_800BF4C0[0xB8] * D_800BF4C0[0xCC] * D_800BF4C0[0xCE] / 100;
        break;
    case 3:
    case 6:
        var_a1 = D_800BF4C0[0xB8] * D_800BF4C0[0xCC] / 10;
        break;
    case 7:
        var_a1 = D_800BF4C0[0xB8] * D_800BF4C0[0xCC] * D_800BF4C0[0xCF] / 100;
        break;
    case 0:
    case 8:
        var_a1 = D_800BF630;
        break;
    }
    var_a2 = 0;
    temp_v1 = arg0->unk4;
    var_a0 = 0;
    switch (temp_v1) {
    case 0:
        var_a2 = -var_a1;
        break;
    case 1:
        var_a0 = -var_a1;
        break;
    case 2:
        var_a2 = var_a1;
        break;
    case 3:
        var_a0 = var_a1;
        break;
    }
    arg0->unk10 = (u16) arg0->unk10 + var_a0;
    arg0->unk14 = (u16) arg0->unk14 + var_a2;
}
