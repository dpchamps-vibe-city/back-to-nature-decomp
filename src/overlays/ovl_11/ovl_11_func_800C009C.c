#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "packet_work.h"


void ovl_11_func_800C034C (u8 *arg0);
s32 ovl_11_func_800F5868 (s32 arg0, s32 arg1);
void func_80015EE8 (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
u32 Rand (s32 arg0);

void ovl_11_func_800C009C(u8 *arg0) {
    s32 temp_v0_3;
    s32 temp_v1;
    s32 var_v0;
    s32 temp_v0_2;
    s32 var_a0;
    s32 var_a2;
    s32 temp_a0;
    s32 temp_a0_2;
    u32 temp_v0;
    u16 temp_v1_2;
    s32 temp_v1_3;
    u16 var_v0_2;
    u32 temp_a1;

    if ((*(s16 *) ((u8 *) arg0 + 8)) >= 0) {
        if (!(D_8006C844 & 0x08000000)) {
            (*(s16 *) ((u8 *) arg0 + 8)) = (s16) ((u16) (*(s16 *) ((u8 *) arg0 + 8)) - 1);
        }
    } else {
        temp_v0 = *(u16 *) (arg0 + 0xC);
        temp_v1 = temp_v0 & 0x4000;
        if (temp_v1) {
            temp_a1 = temp_v0;
            temp_v0 = temp_a1 & 0x1000;
            var_a0 = 5;
            if (temp_v0) {
                var_a2 = 7;
            } else {
                var_a0 = 1;
                var_a2 = 0;
            }
            temp_v1 = *(s16 *) (arg0 + 2);
            if (temp_a1 & 0x2000) {
                var_v0 = temp_v1 + (var_a0 * 2);
            } else {
                var_v0 = temp_v1 + var_a0;
            }
            (*(s16 *) (arg0 + 2)) = var_v0;
            (*(u16 *) (arg0 + 0)) -= var_a2;
            temp_v1_2 = *(u16 *) (arg0 + 0xA);
            temp_v0_2 = (temp_v1_2 & 7) != 7;
            if (temp_v1_2 & 8) {
                (*(u16 *) (arg0 + 0)) += temp_v0_2;
            } else {
                (*(u16 *) (arg0 + 0)) -= temp_v0_2;
            }
            if (!(D_8006C844 & 0x08000000)) {
                temp_v1_3 = *(u16 *) ((u8 *) arg0 + 0xA);
                if ((*(u16 *) ((u8 *) arg0 + 0xC)) & 0x2000) {
                    if (temp_v1_3 >= 0x37) {
                        goto block_21;
                    }
                    goto block_22;
                }
                if (temp_v1_3 >= 0x6E) {
                    goto block_21;
                }
                goto block_22;
            }
            goto block_25;
        }
        temp_a1 = (u16) (*(u16 *) ((u8 *) arg0 + 0xA)) >> 1;
        (*(s16 *) ((u8 *) arg0 + 6)) = (s16) temp_a1;
        if (!(D_8006C844 & 0x08000000)) {
            if ((s32) temp_a1 >= 3) {
block_21:
                ovl_11_func_800C034C(arg0);
                return;
            }
block_22:
            (*(u16 *) ((u8 *) arg0 + 0xA)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0xA)) + 1);
            goto block_24;
        }
        (*(s16 *) ((u8 *) arg0 + 6)) %= 3;
block_24:
block_25:
        temp_a0_2 = (*(u16 *) ((u8 *) arg0 + 0)) + D_800A0384.unk14;
        (*(u16 *) ((u8 *) arg0 + 0)) = temp_a0_2;
        temp_v0_3 = (u16) (*(s16 *) ((u8 *) arg0 + 2)) + D_800A0384.unk16;
        (*(s16 *) ((u8 *) arg0 + 2)) = temp_v0_3;
        if (ovl_11_func_800F5868((s32) (s16) temp_a0_2, (s32) (s16) temp_v0_3) != 0) {
            func_80015EE8(gActiveRenderContext->field_120, (s32) &D_800A0728, (s32) (*(u8 *) ((u8 *) arg0 + 4)), (s32) (u8) (*(s16 *) ((u8 *) arg0 + 6)), (s16) (s32) (s16) (*(u16 *) ((u8 *) arg0 + 0)), (s16) (s32) (*(s16 *) ((u8 *) arg0 + 2)));
        }
    }
}
