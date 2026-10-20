#include "common.h"
#include "game_types.h"

void ovl_11_func_800D12B8(Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1, s32 arg2);
s32 ovl_11_func_800D0408(s16 arg0, Recon800D0408A1View *arg1, s32 arg2);

s32 ovl_11_func_8010AE64(Recon_ovl_11_func_800D2594_A0View *arg0) {
    u16 *var_s1;
    s32 var_a1 = 0x18;

    var_s1 = D_80127B2C;
    if (D_8012D084 != 0) {
        var_s1 += 3;
    }
    if (arg0->unk34 & 0x8000) {
        arg0->unk34 |= 0x800000;
        var_a1 = 0x24;
    }
    ovl_11_func_800D12B8(arg0, var_a1, 0x31);
    arg0->unk34 |= 0x800;
    if ((*(s32 *) ((u8 *) arg0 + 0x48) == 0) && (arg0->unk4C == 0) && (*(s32 *) ((u8 *) arg0 + 0x50) == 0)) {
        if (*(u16 *) ((u8 *) arg0 + 0x7A) & 0x300) {
            *(u16 *) ((u8 *) arg0 + 0x24) = var_s1[0];
        } else {
            *(u16 *) ((u8 *) arg0 + 0x24) = var_s1[1];
        }
    } else if (arg0->unk34 & 0x01000000) {
        *(u16 *) ((u8 *) arg0 + 0x24) = var_s1[2];
    } else {
        *(u16 *) ((u8 *) arg0 + 0x24) = var_s1[1];
    }
    if (*(u16 *) ((u8 *) arg0 + 0x7A) & 0x300) {
        if (!(arg0->unk34 & 0x01000000)) {
            arg0->unk34 &= ~0x800;
        }
    }
    if (!(arg0->unk34 & 0x2000)) {
        *(s16 *) ((u8 *) arg0 + 0x2C) = 0x12C;
        if ((*(u16 *) ((u8 *) arg0 + 0x7A) & 0x300) && !(arg0->unk34 & 0x01000000)) {
            ovl_11_func_800D0408(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
        }
    }
    return 1;
}
