#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void func_80015EE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);

void ovl_27_func_800B944C(void) {
    s16 temp_a1;
    s16 temp_s0;
    s16 temp_s0_2;
    s16 temp_s0_3;
    s16 temp_s0_4;
    s16 temp_s1;
    s16 temp_s1_2;
    s16 temp_s1_3;
    s16 temp_s1_4;
    s16 temp_s2;
    s16 temp_s2_2;
    s16 temp_s3;
    s16 temp_s3_2;
    s16 temp_s4;
    s16 temp_v0;
    s16 temp_v0_2;
    s32 temp_s5;
    s32 var_v0;
    s32 var_v0_3;
    s16 quotient1, quotient2, quotient3;
    s32 next, next2;
    s32 offset1, offset2, offset3, offset4;

    temp_a1 = D_800C4A4C / 240;
    temp_s4 = (temp_a1 * 0xF0) - (u16)D_800C4A4C;
    temp_s1 = 0x140 - (temp_a1 * 0x50);
    temp_s5 = gActiveRenderContext->field_D8 + 0x90;
    temp_s1 += ccos((D_800C4A4E << 12) / 120) * 12 / 4096;
    temp_s3 = temp_s4 - temp_s1;
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s3, (s16) (s32) temp_s4);
    offset1 = temp_s1 - 0x140;
    temp_s2 = temp_s4 - offset1;
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s2, (s16) (s32) temp_s4);
    offset2 = temp_s1 - 0x280;
    temp_s1_2 = temp_s4 - offset2;
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s1_2, (s16) (s32) temp_s4);
    temp_s0 = temp_s4 + 0xF0;
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s3, (s16) (s32) temp_s0);
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s2, (s16) (s32) temp_s0);
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s1_2, (s16) (s32) temp_s0);
    temp_s0_2 = temp_s4 + 0x1E0;
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s3, (s16) (s32) temp_s0_2);
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s2, (s16) (s32) temp_s0_2);
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s1_2, (s16) (s32) temp_s0_2);
    if (D_8006C840 & 1) {
        next = (u16)D_800C4A4C;
        next++;
        quotient1 = (s16)next / 960;
        next -= quotient1 * 960;
        D_800C4A4C = next;
    }
    quotient2 = D_800C4A4E / 240;
    temp_s4 = (u16)D_800C4A4E - quotient2 * 240;
    temp_s1_3 = 0x140 - quotient2 * 0x50;
    temp_s1_3 += csin((D_800C4A4E << 12) / 120) * 12 / 4096;
    temp_s3_2 = temp_s4 + temp_s1_3;
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s3_2, (s16) (s32) temp_s4);
    offset3 = temp_s1_3 - 0x140;
    temp_s2_2 = temp_s4 + offset3;
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s2_2, (s16) (s32) temp_s4);
    offset4 = temp_s1_3 - 0x280;
    temp_s1_4 = temp_s4 + offset4;
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s1_4, (s16) (s32) temp_s4);
    temp_s0_3 = temp_s4 - 0xF0;
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s3_2, (s16) (s32) temp_s0_3);
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s2_2, (s16) (s32) temp_s0_3);
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s1_4, (s16) (s32) temp_s0_3);
    temp_s0_4 = temp_s4 - 0x1E0;
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s3_2, (s16) (s32) temp_s0_4);
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s2_2, (s16) (s32) temp_s0_4);
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x25, 0, (s16) (s32) temp_s1_4, (s16) (s32) temp_s0_4);
    next2 = (u16)D_800C4A4E;
    next2++;
    quotient3 = (s16)next2 / 960;
    next2 -= quotient3 * 960;
    temp_s5 = gActiveRenderContext->field_D8 + 0x94;
    D_800C4A4E = next2;
    func_80015EE8(temp_s5, (s32) D_800C4BD0, 0x1D, 0, 0, 0);
}
