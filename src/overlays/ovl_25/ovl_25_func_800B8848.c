#include "common.h"
#include "game_types.h"

s32 ovl_25_func_800B9758(Ovl25StateSelectorView *arg0);
/* The state update has a byte-exact void body as well as the existing
 * value-returning reconstruction. This caller requires the void contract;
 * none of its calls consumes a result. */
void ovl_25_func_800B9B00(s16 *arg0, s16 arg1, s16 arg2);
s32 ovl_25_func_800B9A10 (s32 arg0);
void func_8001AC10 (u32 *arg0, s32 arg1, s32 arg2);

void ovl_25_func_800B8848(void) {
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_v0;
    u8 *temp_s1;
    u8 *temp_v0_2;
    Ovl25StateView *state;

    if (D_800BFE54.unk2 != 8) {
        temp_v0 = ovl_25_func_800B9758((Ovl25StateSelectorView *) &D_800BFE54);
        if (temp_v0 == 1) {
            if ((*(s32 *) ((u8 *) D_8005E3A8 + 4)) & 0x10) {
                (*(s16 *) ((u8 *) &D_800BFE54 + -2)) = 0;
                ovl_25_func_800B9B00(&D_800BFE54.unk0, 5, 1);
            } else {
                if ((*(s32 *) ((u8 *) D_8005E3A8 + 8)) & 0x40) {
                    temp_s1 = (u8 *) &D_800BFE54 - 0x10;
                    *(s16 *)(temp_s1 + 0xE) = 0;
                    if ((*(s32 *) ((u8 *) &D_800BFE54 + 0xC)) <= 0x9FFF) {
                        ovl_25_func_800B9B00(&D_800BFE54.unk0, 7, 0x3C);
                        D_800BCD21 = temp_v0;
                        D_800BCD48[1] = 0;
                        D_800BCD48[0] = D_800BFE54.source[0] - 0x320;
                        D_800BCD48[2] = D_800BFE54.source[2];
                    } else {
                        ovl_25_func_800B9B00(&D_800BFE54.unk0, 4, *(s16 *)(temp_s1 + 0x4D4));
                        *(s16 *)(temp_s1 + 0x18) = ovl_25_func_800B9A10(*(s32 *)(temp_s1 + 0x1C));
                    }
                } else {
                    temp_v0_2 = (u8 *) &D_800BFE54 - 0x10;
                    (*(u16 *)(temp_v0_2 + 0xE))++;
                    if ((u32) ((u16) D_800BFE54.unk2 - 4) < 2U) {
                        if ((s16) D_800BFE54.unk4 == 0) {
                            ovl_25_func_800B9B00(&D_800BFE54.unk0, 2, 1);
                        }
                    }
                }
            }
        }
        state = &D_800BFE44_view;
        if (state->unk4CC != 0) {
            if ((*(s32 *) ((u8 *) D_8005E3A8 + 8)) & 0x50) {
                state->unk4CC = 0;
                return;
            }
            if (state->unk4CC == 1) {
                temp_a1 = D_8005E3C0->field_D8;
                func_8001AC10((u32 *)(temp_a1 + 0xC), temp_a1 + 8, (s32) D_800BCB34);
                return;
            }
            if (state->unk4CC == -1) {
                temp_a1_2 = D_8005E3C0->field_D8;
                func_8001AC10((u32 *)(temp_a1_2 + 0xC), temp_a1_2 + 8, (s32) D_800BCB8E);
            }
        }
    }
}
