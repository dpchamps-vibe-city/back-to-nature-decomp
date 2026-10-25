#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void func_8001AC10 (u32 *arg0, s32 arg1, s32 arg2);
s16 ovl_15_func_80133538 (u8 *arg0, s16 arg1);
u32 func_80017A64 (void);
void func_80017A48 (u32 arg0);
void func_80022580 (u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_15_func_801320A8(void) {
    s16 sp10[2];
    s16 sp18[2];
    s16 sp20[2];
    s16 sp28[2];
    s16 sp30[2];
    s16 sp38[2];
    s16 sp40[2];
    s16 *var_a2;
    s16 var_a0;
    s16 var_a0_2;
    s16 var_v1;
    s16 var_v1_2;
    s32 *temp_s0;
    s32 temp_a1;
    s32 temp_t2;
    s32 var_t1;
    s32 var_t1_2;
    s16 *temp_a0;
    s16 *temp_a1_2;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32) ((void *) ((u8 *) &D_800520A0 + *gTextBankOffsets)));
    for (var_t1 = 0; var_t1 < 2; var_t1++) {
        sp10[var_t1] = sp18[var_t1] = sp20[var_t1] = 0;
        for (var_t1_2 = 0; var_t1_2 < 5; var_t1_2++) {
            if (Ovl15Slots[var_t1][var_t1_2].unk0 == 0) {
                sp18[var_t1] += 1;
            }
            if (Ovl15Slots[var_t1][var_t1_2].unk0 == 1) {
                sp20[var_t1] += 1;
            }
            if (Ovl15Slots[var_t1][var_t1_2].unk0 == 2) {
                sp20[var_t1] += 1;
            }
        }
        sp10[var_t1] = sp18[var_t1] + sp20[var_t1];
    }
    sp28[0] = ovl_15_func_80133538((u8 *) &D_800742EC, 0);
    sp28[1] = ovl_15_func_80133538(D_801376D0, 0);
    sp30[0] = ovl_15_func_80133538((u8 *) &D_800742EC, 1);
    temp_s0 = &D_800742EC - 0x1EAD;
    sp30[1] = ovl_15_func_80133538(D_801376D0, 1);
    var_a0 = 0xA;
    if ((*(u16 *) ((u8 *) temp_s0 + 0x44D2)) == 0) {
        var_a0 = 5;
    }
    sp38[0] = var_a0;
    var_a0_2 = 0xA;
    if ((*(u16 *) ((u8 *) D_801376D0 + 0x236A)) == 0) {
        var_a0_2 = 5;
    }
    var_v1 = 0x14;
    sp38[1] = var_a0_2;
    if ((*(u16 *) ((u8 *) temp_s0 + 0x44D0)) == 0) {
        var_v1 = 0xA;
    }
    sp40[0] = var_v1;
    var_v1_2 = 0x14;
    if ((*(u16 *) ((u8 *) D_801376D0 + 0x2368)) == 0) {
        var_v1_2 = 0xA;
    }
    sp40[1] = var_v1_2;
    if ((sp10[0] == 0) && (sp10[1] == 0)) {
        D_80137584 = 0x15;
        return;
    }
    if (sp38[0] < ((sp28[0] - sp18[0]) + sp18[1])) {
        D_80137584 = 0x16;
        return;
    }
    if (sp38[1] < ((sp28[1] - sp18[1]) + sp18[0])) {
        D_80137584 = 0x17;
        return;
    }
    if (sp40[0] < ((sp30[0] - sp20[0]) + sp20[1])) {
        D_80137584 = 0x18;
        return;
    }
    if (sp40[1] < ((sp30[1] - sp20[1]) + sp20[0])) {
        D_80137584 = 0x19;
        return;
    }
    D_80137588 = 1;
    D_80137584 = 0x1A;
}
