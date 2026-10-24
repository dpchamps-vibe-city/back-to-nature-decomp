#include "common.h"
#include "game_types.h"

void ovl_11_func_800DE3A0(void);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800CD734(Ovl11C7010Object *arg0, s32 arg1) {
    s32 temp_a1;
    s32 value;
    s32 var_v0;
    u16 temp_v1;
    u16 temp_v1_2;
    u16 temp_v1_3;
    u16 temp_v1_4;
    u16 temp_v1_5;
    u16 temp_v1_6;
    u16 temp_v1_7;
    u16 var_s1;

    var_s1 = 0;
    value = (s16) (*(u16 *) ((u8 *) arg0 + 0x14));
    temp_a1 = value / 10;
    if ((*(s16 *) ((u8 *) arg0 + 0x12)) == 0) {
        temp_v1 = *(u16 *) ((u8 *) arg0 + 0x36);
        if (!(temp_v1 & 0x1000)) {
            (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) (temp_v1 | 0x1000);
            var_s1 = 0x40;
        }
    } else {
        (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0x36)) & 0xEFFF);
    }
    if ((*(s16 *) ((u8 *) arg0 + 0x12)) < temp_a1) {
        temp_v1_2 = *(u16 *) ((u8 *) arg0 + 0x36);
        if (!(temp_v1_2 & 0x3000)) {
            (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) (temp_v1_2 | 0x2000);
            var_s1 = 0x3F;
        }
    } else {
        (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0x36)) & 0xCFFF);
    }
    if ((*(s16 *) ((u8 *) arg0 + 0x12)) < (temp_a1 * 3)) {
        temp_v1_3 = *(u16 *) ((u8 *) arg0 + 0x36);
        if (!(temp_v1_3 & 0x7000)) {
            (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) (temp_v1_3 | 0x4000);
            var_s1 = 0x3E;
        }
    } else {
        (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0x36)) & 0x8FFF);
    }
    if ((*(s16 *) ((u8 *) arg0 + 0x12)) < (temp_a1 * 5)) {
        temp_v1_4 = *(u16 *) ((u8 *) arg0 + 0x36);
        if (!(temp_v1_4 & 0xF000)) {
            (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) (temp_v1_4 | 0x8000);
            var_s1 = 0x3D;
        }
    } else {
        (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0x36)) & 0xFFF);
    }
    if ((*(s16 *) ((u8 *) arg0 + 0x16)) == 0x64) {
        temp_v1_5 = *(u16 *) ((u8 *) arg0 + 0x36);
        if (!(temp_v1_5 & 0x200)) {
            (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) (temp_v1_5 | 0x200);
            var_s1 = 0x40;
            ovl_11_func_800DE3A0();
        }
    } else {
        (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0x36)) & 0xFDFF);
    }
    if ((*(s16 *) ((u8 *) arg0 + 0x16)) >= 0x50) {
        temp_v1_6 = *(u16 *) ((u8 *) arg0 + 0x36);
        if (!(temp_v1_6 & 0x600)) {
            (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) (temp_v1_6 | 0x400);
            var_s1 = 0x43;
        }
    } else {
        (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0x36)) & 0xF9FF);
    }
    if ((*(s16 *) ((u8 *) arg0 + 0x16)) >= 0x32) {
        temp_v1_7 = *(u16 *) ((u8 *) arg0 + 0x36);
        if (!(temp_v1_7 & 0xE00)) {
            (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) (temp_v1_7 | 0x800);
            var_s1 = 0x42;
        }
    } else {
        (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0x36)) & 0xF1FF);
    }
    var_v0 = 0;
    if ((arg1 == 0) && (var_s1 != 0)) {
        arg0->unk6C |= 0x5800;
        ovl_11_func_800C70CC(arg0, var_s1, 0, -2);
        var_v0 = 1;
    }
    return var_v0;
}
