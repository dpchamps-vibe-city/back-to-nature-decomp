#include "common.h"
#include "game_types.h"
#include "ovl11_object_record_state.h"

/* This call also supplies an unused fourth word, zero, in the original. */
s32 ovl_11_func_800E8C14();
s16 ovl_11_func_800F0734(s16 arg0, s32 arg1, s16 arg2);

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

    temp_s5 = (s16) gOvl11ValueSlots[arg1];
    temp_s2 = (Recon_ovl_11_func_8010CE80_A0View *)(gObjectRecordPool + temp_s5 * 0xF8);
    temp_s2->unk34 |= 8;
    if (ovl_11_func_800E8C14(0x1B, arg0, 0, 0) == 0 || (table = &gOvl11ValueSlots[arg0], temp_v1 = *table) == 0) {
        index = temp_s5 + 0xB;
        entry = &gOvl11ValueSlots[index];
        if (*entry != 2) {
            *entry = 1;
            gOvl11ValueSlots[arg2] = 1;
            gOvl11ValueSlots[arg0] = 0xB;
        } else {
            gOvl11ValueSlots[arg0] = 0xC;
        }
        return 1;
    }
    if (temp_v1 >= 4) {
        goto failure;
    }
    if (temp_v1 <= 0) {
        goto failure;
    }
    if (gOvl11ValueSlots[arg2] != 0) {
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
        table = gOvl11ValueSlots;
        index = temp_s5 + 0xB;
        table[index] = 2;
        for (i = 0; i < 7; i++) {
            if (gOvl11ValueSlots[i + 0xB] != 2) {
                break;
            }
        }
        if (i != 7) {
            gOvl11ValueSlots[arg0] = 0xD;
        } else {
            gOvl11ValueSlots[arg0] = 0xE;
            temp_s2 = (Recon_ovl_11_func_8010CE80_A0View *)gObjectRecordPool;
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
    Ovl11SetObjectRecordMode(temp_s2, 6);
    return 0;
}
