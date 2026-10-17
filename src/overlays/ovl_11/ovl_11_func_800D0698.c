#include "common.h"
#include "game_types.h"
#include "psyq/abs.h"

s32 ovl_11_func_800D062C(Recon_ovl_11_func_800D2594_A0View *arg0);

void ovl_11_func_800D0698(Recon_ovl_11_func_800D2594_A0View *arg0) {
    s32 var_s0;
    s32 var_v0;
    s32 temp_v0;

    var_s0 = abs((*(s32 *) ((u8 *) arg0 + 0x38)) - (*(s32 *) ((u8 *) arg0 + 0x58)));
    var_v0 = abs(arg0->unk3C - (*(s32 *) ((u8 *) arg0 + 0x5C)));
    var_s0 = var_v0 < var_s0;
    temp_v0 = ovl_11_func_800D062C(arg0);
    switch (temp_v0) {
    case 1:
    case 5:
        (*(s16 *) ((u8 *) arg0 + 0x22)) = var_s0 ? 3 : 2;
        break;
    case 2:
    case 6:
        (*(s16 *) ((u8 *) arg0 + 0x22)) = var_s0 ? 3 : 0;
        break;
    case 3:
    case 7:
        (*(s16 *) ((u8 *) arg0 + 0x22)) = var_s0 ? 1 : 0;
        break;
    case 4:
    case 8:
        (*(s16 *) ((u8 *) arg0 + 0x22)) = var_s0 ? 1 : 2;
        break;
    }
}
