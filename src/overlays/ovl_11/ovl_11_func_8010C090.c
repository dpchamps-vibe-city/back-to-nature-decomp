#include "common.h"
#include "game_types.h"

void ovl_11_func_800D12B8(Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1, s32 arg2);
s32 ovl_11_func_800D0408(s16 arg0, Recon800D0408A1View *arg1, s32 arg2);

s32 ovl_11_func_8010C090(Recon_ovl_11_func_800D2594_A0View *arg0) {
    s32 temp_a0;
    s32 temp_v1;
    s32 temp_v1_2;

    temp_v1 = arg0->unk34;
    if (temp_v1 & 0x8000) {
        arg0->unk34 = temp_v1 | 0x800000;
    }
    ovl_11_func_800D12B8(arg0, 0x18, 0x8031);
    arg0->unk34 |= 0x800;
    if (((*(s32 *) ((u8 *) arg0 + 0x48)) == 0) && (arg0->unk4C == 0) && ((*(s32 *) ((u8 *) arg0 + 0x50)) == 0)) {
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 0;
        if (!((*(u16 *) ((u8 *) arg0 + 0x7A)) & 0x300)) {
            (*(s16 *) ((u8 *) arg0 + 0x24)) = 3;
        }
    } else {
        if (arg0->unk34 & 0x01000000) {
            (*(s16 *) ((u8 *) arg0 + 0x24)) = 4;
        } else {
            (*(s16 *) ((u8 *) arg0 + 0x24)) = 3;
        }
    }
    if ((*(u16 *) ((u8 *) arg0 + 0x7A)) & 0x300) {
        temp_v1_2 = arg0->unk34;
        if (!(temp_v1_2 & 0x01000000)) {
            arg0->unk34 = temp_v1_2 & ~0x800;
        }
    }
    temp_a0 = arg0->unk34;
    if (!(temp_a0 & 0x2000)) {
        (*(s16 *) ((u8 *) arg0 + 0x2C)) = 0x12C;
        if (((*(u16 *) ((u8 *) arg0 + 0x7A)) & 0x300) && !(temp_a0 & 0x01000000)) {
            ovl_11_func_800D0408(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
        }
    }
    return 1;
}
