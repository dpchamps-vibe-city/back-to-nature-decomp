#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 ovl_11_func_800BE118(VECTOR position, s16 arg4, VECTOR *arg5, u16 *arg6, u32 arg7);
void ovl_11_func_800D0408(s16 arg0, Recon800D0408A1View *arg1, s32 arg2);
s32 ovl_11_func_80107C80(s16 arg0, u16 arg1, s32 *arg2, s32 arg3);
u16 ovl_11_func_8010734C(s32 id);

void ovl_11_func_800D2B40(Recon_ovl_11_func_800D2594_A0View *arg0) {
    VECTOR sp20;
    u16 sp30;
    s16 temp_s0;
    s16 var_s2;
    s16 var_s4;
    s32 var_s0;
    s32 var_v1;
    u16 var_s3;
    u32 var_t0;
    u8 *work;

    for (var_s0 = 0; var_s0 < 4; var_s0++) {
        work = (u8 *) &D_8007AFF0;
        var_s2 = 1;
        switch (var_s0) {
        case 0:
            var_v1 = 0x04000000;
            var_t0 = 0;
            var_s3 = 2;
            var_s4 = 2;
            break;
        case 1:
            var_v1 = 0x08000000;
            var_t0 = 1;
            var_s2 = 3;
            var_s3 = 2;
            var_s4 = 2;
            break;
        case 2:
            if (D_80070D0C == 0) {
                continue;
            }
            var_v1 = 0x10000000;
            var_t0 = 2;
            var_s2 = 4;
            var_s3 = 1;
            var_s4 = 1;
            break;
        case 3:
            var_v1 = 0x20000000;
            var_t0 = 3;
            var_s2 = 2;
            var_s3 = 1;
            var_s4 = 1;
            break;
        default:
            continue;
        }
        if ((arg0->unk34 & var_v1) && (ovl_11_func_800BE118(*(VECTOR *) ((u8 *) arg0 + 0x38), *(s16 *) ((u8 *) arg0 + 0x30), &sp20, &sp30, var_t0) != 0)) {
            temp_s0 = *(s16 *) (work + 0x25476);
            if (temp_s0 == 1) {
                ovl_11_func_80107C80(var_s2, var_s3, NULL, 1);
                (*(s16 *) ((u8 *) arg0 + 0x2C)) = 0;
                if ((*(s16 *) ((u8 *) arg0 + 0x30)) == temp_s0) {
                    (*(s16 *) ((u8 *) arg0 + 0x22)) = var_s4;
                } else {
                    ovl_11_func_800D0408(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
                }
                if (ovl_11_func_8010734C((s32) var_s2) == 3) {
                    (*(s32 *) ((u8 *) arg0 + 0x38)) = sp20.vx;
                    (*(s32 *) ((u8 *) arg0 + 0x40)) = sp20.vz;
                    (*(s16 *) ((u8 *) arg0 + 0x30)) = (s16) sp30;
                    ovl_11_func_800D0408(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
                    arg0->unk34 &= ~0x2000;
                    (*(s16 *) ((u8 *) arg0 + 0x18)) = 0x5A;
                }
            } else {
                (*(s32 *) ((u8 *) arg0 + 0x38)) = sp20.vx;
                (*(s32 *) ((u8 *) arg0 + 0x40)) = sp20.vz;
                (*(s16 *) ((u8 *) arg0 + 0x30)) = (s16) sp30;
                ovl_11_func_800D0408(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
                arg0->unk34 &= ~0x2000;
            }
            break;
        }
    }
}
