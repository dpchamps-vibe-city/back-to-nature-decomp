#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800BE208(s32 *arg0, Ovl11CollisionTag arg1, u16 arg2, u16 arg3, void **arg4, s32 *arg5, s32 arg6, s32 arg7);
s32 ovl_11_func_800C3020(Ovl11C3B78Arg0 *arg0);
s32 ovl_11_func_80107DEC(Ov11Timer *arg0);
void ovl_11_func_800C2C74(Ovl11C3B78Arg0 *arg0);

void ovl_11_func_800C2CE8(Ovl11C3B78Arg0 *arg0, s32 arg1) {
    s32 position[3];
    void *sp30;
    s32 sp34;
    s32 var_a2;
    s32 var_s1;
    u16 temp_a1;

    if ((*(s16 *) ((u8 *) arg0 + 0x12)) != 0) {
        (*(s16 *) ((u8 *) arg0 + 0x12)) = (s16) ((u16) (*(s16 *) ((u8 *) arg0 + 0x12)) - 1);
    }
    temp_a1 = *(u16 *) ((u8 *) arg0 + 0x1E);
    if (temp_a1 & 0x100) {
        var_a2 = 1;
        if (temp_a1 & 0x1000) {
            position[0] = *(s32 *) ((u8 *) arg0 + 0x60);
            position[1] = *(s32 *) ((u8 *) arg0 + 0x64);
            position[2] = *(s32 *) ((u8 *) arg0 + 0x68);
        } else {
            position[0] = arg0->unk30;
            position[1] = arg0->unk34;
            position[2] = arg0->unk38;
            if (temp_a1 & 1) {
                var_a2 = 0;
            }
            if ((*(s16 *) ((u8 *) arg0 + 0xA)) >= 0x1E) {
                var_a2 = 0;
            }
            if ((*(u16 *) ((u8 *) arg0 + 0x20)) & 0x1000) {
                var_a2 = 0;
            }
        }
        if ((*(u16 *) ((u8 *) arg0 + 0x1E)) & 1) {
            var_a2 = 0;
        }
        if (D_80071A3C == 4) {
            var_a2 = 0;
        }
        if ((var_a2 != 0) && (ovl_11_func_800BE208(position, *(Ovl11CollisionTag *)((u8 *)arg0 + 0x2C), 0x3E8U, 0x3E8U, &sp30, &sp34, 0, 1) != 0) && ((*(s16 *) ((u8 *) arg0 + 0xA)) < 0x1E)) {
            (*(u16 *) ((u8 *) arg0 + 0x1E)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0x1E)) | 4);
        }
    }
    if ((((*(u16 *) ((u8 *) arg0 + 0x1A)) == 3) || ((*(u16 *) ((u8 *) arg0 + 0x1E)) & 4)) && (D_8006C844 & 0x100000)) {
        if (++*(s16 *)((u8 *)arg0 + 0xA) >= 0x1E) {
            (*(u16 *) ((u8 *) arg0 + 0x1E)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0x1E)) & 0xFFFB);
            ovl_11_func_800C3020(arg0);
        }
    }
    for (var_s1 = 0; var_s1 < arg1; var_s1++) {
        ovl_11_func_80107DEC((Ov11Timer *) ((u8 *) arg0 + 0xE0));
        if (*(s16 *)((u8 *)arg0 + 8) != -2) {
            if (*(s16 *)((u8 *)arg0 + 8) != -3) {
                if (*(s16 *)((u8 *)arg0 + 8) != -2) {
                    --*(s16 *)((u8 *)arg0 + 8);
                }
            }
            if (*(s16 *)((u8 *)arg0 + 8) == -1) {
                ovl_11_func_800C2C74(arg0);
            }
        } else {
            ovl_11_func_800C3020(arg0);
            if (!(*(u16 *)((u8 *)arg0 + 0x1E) & 0x100)) {
                break;
            }
        }
        if (D_80128B68 == 0) {
            D_80122F88 += 1;
        }
    }
}
