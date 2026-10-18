#include "common.h"
#include "game_types.h"

/* This call also supplies an unused fourth word, zero, in the original. */
s32 ovl_11_func_800E8C14();
s16 ovl_11_func_800F0734(s16 arg0, s32 arg1, s16 arg2);
s32 ovl_11_func_8010CE80(Recon_ovl_11_func_8010CE80_A0View *arg0, s32 arg1);

s32 ovl_11_func_800EDAA4(s16 arg0, s16 arg1, s32 arg2) {
    Recon_ovl_11_func_8010CE80_A0View *temp_s2;
    s16 temp_s5;
    s16 temp_v0_2;
    s16 i;
    s32 var_a1;
    s32 temp_v1;
    s32 index;
    s32 *table;
    s32 *entry;

    temp_s5 = (s16) D_80129560[arg1];
    temp_s2 = (Recon_ovl_11_func_8010CE80_A0View *)(D_8007A638 + temp_s5 * 0xF8);
    temp_s2->unk34 |= 8;
    if (ovl_11_func_800E8C14(0x1B, arg0, 0, 0) == 0 || (table = &D_80129560[arg0], temp_v1 = *table) == 0) {
        index = temp_s5 + 0xB;
        entry = &D_80129560[index];
        if (*entry != 2) {
            *entry = 1;
            D_80129560[arg2] = 1;
            D_80129560[arg0] = 0xB;
        } else {
            D_80129560[arg0] = 0xC;
        }
        return 1;
    }
    if (temp_v1 >= 4) {
        goto failure;
    }
    if (temp_v1 <= 0) {
        goto failure;
    }
    if (D_80129560[arg2] != 0) {
        if (!(temp_s2->unkB8 & 0x10)) {
            temp_s2->unkB8 |= 0x10;
            temp_v0_2 = ovl_11_func_800F0734(temp_s5 + 0x32, 0, *table - 1);
        } else {
            temp_v0_2 = ovl_11_func_800F0734(temp_s5 + 0x32, 2, *table - 1);
        }
        if (*(s16 *)((u8 *)temp_s2 + 0x16) < 0xFF - temp_v0_2) {
            *(s16 *)((u8 *)temp_s2 + 0x16) += temp_v0_2;
        } else {
            *(s16 *)((u8 *)temp_s2 + 0x16) = 0xFF;
        }
        table = D_80129560;
        index = temp_s5 + 0xB;
        table[index] = 2;
        for (i = 0; i < 7; i++) {
            if (D_80129560[i + 0xB] != 2) {
                break;
            }
        }
        if (i != 7) {
            D_80129560[arg0] = 0xD;
        } else {
            D_80129560[arg0] = 0xE;
            temp_s2 = (Recon_ovl_11_func_8010CE80_A0View *)D_8007A638;
            for (i = 0; i < 7; i++, temp_s2 = (Recon_ovl_11_func_8010CE80_A0View *)((u8 *)temp_s2 + 0xF8)) {
                if (*(s16 *)((u8 *)temp_s2 + 0x16) < 0xF5) {
                    *(s16 *)((u8 *)temp_s2 + 0x16) += 0xA;
                } else {
                    *(s16 *)((u8 *)temp_s2 + 0x16) = 0xFF;
                }
            }
        }
        return 1;
    }
failure:
    ovl_11_func_8010CE80(temp_s2, 6);
    return 0;
}
