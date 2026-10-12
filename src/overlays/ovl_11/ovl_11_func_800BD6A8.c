#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"

void func_8001AF70(u16 arg0, u16 arg1);
void func_80014BCC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8001719C(u8 *arg0);

void ovl_11_func_800BD6A8(s16 arg0, s16 arg1) {
    s16 var_s0;
    s32 *temp_v1;
    s32 baseIndex;
    s32 wordOffset;
    s32 temp_a1;
    s32 temp_s0;
    s32 temp_s1;
    s32 var_s3;
    s32 *temp_s2;

    var_s0 = arg0;
    var_s3 = 0;
    if (var_s0 != -1) {
        if (var_s0 == 0xE) {
            var_s0 = (s16) ((struct Ovl11SpriteSelectors *) D_8006C838)->hour / 6;
            if (var_s0 >= 3) {
                var_s0 = 2;
            }
            if (var_s0 == 1) {
                func_8001AF70(0x3FU, 1U);
            }
            if (var_s0 == 2) {
                func_8001AF70(0x40U, 1U);
            }
            var_s3 = 1;
            var_s0 = var_s0 + (s16) ((struct Ovl11SpriteSelectors *) D_8006C838)->day * 3;
            var_s0 += 0xE;
        }
        if (var_s0 == 0x3E6) {
            if (((struct Ovl11SpriteSelectors *) D_8006C838)->season != -1) {
                var_s0 = ((struct Ovl11SpriteSelectors *) D_8006C838)->season * 3 + 0x23;
                baseIndex = var_s0;
                baseIndex += (u16) ((struct Ovl11SpriteSelectors *) D_8006C838)->variant;
                var_s0 = baseIndex;
                var_s3 = 2;
                goto block_12;
            }
        } else {
block_12:
            temp_v1 = D_80125DC0[var_s0];
            wordOffset = *temp_v1;
            temp_s0 = temp_v1[wordOffset + 1];
            temp_s2 = &D_80122810[var_s0];
            temp_s1 = D_8005E3B0 + 0x4290;
            if (arg1 != 0) {
                DrawSync(0);
            }
            temp_a1 = temp_s2[0];
            func_80014BCC(0, temp_a1, temp_s2[1] - temp_a1, 0, temp_s1);
            if (var_s3 == 1) {
                memcpy(D_8008F7F8_words, (void *) temp_s1, temp_s0);
            } else {
                memcpy(&D_8009CBF8, (void *) temp_s1, temp_s0);
            }
            func_8001719C((u8 *) (temp_s1 + temp_s0));
            if (var_s3 == 2) {
                temp_v1 = D_80125DC0[var_s0];
                wordOffset = *temp_v1;
                temp_s0 = temp_v1[wordOffset + 2];
                func_8001719C((u8 *) (temp_s1 + temp_s0));
            }
        }
    }
}
