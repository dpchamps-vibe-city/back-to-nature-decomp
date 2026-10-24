#include "common.h"
#include "game_types.h"

void ovl_11_func_800D12B8(Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1, s32 arg2);

s32 ovl_11_func_800E0280(Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1) {
    s32 temp_s1;
    s32 var_a1;
    s32 var_a2;
    s32 temp_a0;

    temp_s1 = (*(s16 *) ((u8 *) arg0 + 0x26)) == 0xD;
    temp_a0 = *(u16 *) ((u8 *) arg0 + 0xB2);
    var_a1 = temp_a0 ? 2 : 6;
    var_a2 = 0x8031;
    if (temp_s1 != 0) {
        var_a2 = 0x8030;
        if (temp_a0 == 0) {
            arg0->unk34 |= 0x04000000;
        }
    } else if ((((struct Ovl11HandlerState *)D_8006C838)->field_52C6 == 5) && (((struct Ovl11HandlerState *)D_8006C838)->field_E4C8 == 0)) {
        if (temp_a0 == 0) {
            arg0->unk34 |= 0x04000000;
        }
    } else {
        arg0->unk34 &= 0xFBFFFFFF;
    }
    ovl_11_func_800D12B8(arg0, var_a1, var_a2);
    if (((*(s32 *) ((u8 *) arg0 + 0x48)) == 0) && (arg0->unk4C == 0) && ((*(s32 *) ((u8 *) arg0 + 0x50)) == 0)) {
        if (temp_s1 != 0) {
            arg0->unk34 &= ~0x2000;
        }
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 0;
    } else {
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 1;
    }
    arg0->unk34 &= ~0x800;
    return 1;
}
