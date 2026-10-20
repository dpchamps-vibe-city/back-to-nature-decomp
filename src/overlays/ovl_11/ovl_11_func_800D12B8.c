#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 func_80012CB4 (s32 arg0, s32 *arg1, s32 arg2);
void ovl_11_func_800D1E1C (Recon_ovl_11_func_800D2594_A0View *arg0);
void ovl_11_func_800D0408 (s16 arg0, Recon800D0408A1View *arg1, s32 arg2);
s32 ovl_11_func_800D2DB8 (Ovl11Func800BDCE8Arg1 *arg0);
s32 ovl_11_func_800D24FC (Recon_ovl_11_func_800D2594_A0View *arg0, VECTOR position, s16 arg5, s32 arg6);
s32 ovl_11_func_800BE208 (s32 *arg0, Ovl11CollisionTag arg1, u16 arg2, u16 arg3, void **arg4, s32 *arg5, s32 arg6, s32 arg7);
s32 ovl_11_func_800D196C (Recon_ovl_11_func_800D2594_A0View *arg0, u16 arg1, s32 arg2);
s32 ovl_11_func_800D1CFC (Recon_ovl_11_func_800D2594_A0View *arg0);
void ovl_11_func_800D0698 (Recon_ovl_11_func_800D2594_A0View *arg0);
void ovl_11_func_800D2B40 (Recon_ovl_11_func_800D2594_A0View *arg0);
s32 ovl_11_func_800D2594 (Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1);

void ovl_11_func_800D12B8(Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1, s32 arg2) {
    VECTOR sp20;                       /* compiler-managed */
    void *sp30;
    s32 sp34;
    ItemData *temp_v1;
    s16 *world;
    s16 temp_v0;
    s32 temp_v0_3;
    s16 var_s1;
    s32 temp_a2;
    s32 temp_v1_2;
    s32 var_s4;
    u16 temp_s6;
    u16 temp_s7;
    u16 temp_v0_2;
    u16 item;
    u16 var_s3;

    var_s4 = arg2;
    if (func_80012CB4(0x96, &D_80123A14, 1) == 1) {
        D_80128D58 = D_80128D58 != 1;
    }
    temp_v0_3 = (D_80128D58 == 1 ? 0x438 : 0x2D0) / arg1;
    ovl_11_func_800D1E1C(arg0);
    if (arg0->unk34 & 0x200000) {
        var_s4 &= ~0x30;
    }
    if ((*(s16 *) ((u8 *) arg0 + 0x18)) != 0) {
        --(*(s16 *) ((u8 *) arg0 + 0x18));
        if ((*(s16 *) ((u8 *) arg0 + 0x18)) < 0) {
            (*(s16 *) ((u8 *) arg0 + 0x18)) = 0;
        }
        var_s4 &= ~1;
    }
    if ((*(u16 *) ((u8 *) arg0 + 0)) == 0x15B) {
        D_80128D6C = 0;
    } else {
        D_80128D6C = 1;
    }
    world = (s16 *) &D_8007AFF0;
    if (((*(s16 *) ((u8 *) arg0 + 0x30)) != world[0x12A3B]) && (arg0->unk34 & 0x200000)) {
        ovl_11_func_800D0408(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
        return;
    }
    temp_a2 = arg0->unk34;
    if (temp_a2 & 0x800000) {
        var_s4 &= ~0x20;
    }
    item = *(u16 *) arg0;
    temp_s7 = *(u16 *) ((u8 *) &D_8006C858[item] + 0x24);
    temp_s6 = *(u16 *) ((u8 *) &D_8006C858[item] + 0x26);
    ++(*(s16 *) ((u8 *) arg0 + 0x2E));
    if (((*(s16 *) ((u8 *) arg0 + 0x2E)) >= temp_v0_3) && !(temp_a2 & 0x01000000)) {
        var_s3 = 0;
        (*(u16 *) ((u8 *) arg0 + 0x2E)) = 0U;
        ovl_11_func_800D0408(*(s16 *) ((u8 *) arg0 + 0x22), (Recon800D0408A1View *) &sp20, arg1);
        sp20.vx +=  (*(s32 *) ((u8 *) arg0 + 0x38));
        sp20.vz += *(s32 *) ((u8 *) arg0 + 0x40);
        if (arg0->unk34 & 0x200000) {
            var_s3 = ovl_11_func_800D2DB8((Ovl11Func800BDCE8Arg1 *) &sp20) == 0;
        }
        if (arg0->unk34 & 0x800000) {
            if (ovl_11_func_800D24FC(arg0, sp20, (s16) (s32) (*(s16 *) ((u8 *) arg0 + 0x22)), 0) != 1) {
                var_s3 |= 1;
            }
        }
        if (ovl_11_func_800BE208(&sp20.vx, *(Ovl11CollisionTag *) ((u8 *) arg0 + 0x2E), temp_s7, temp_s6, &sp30, &sp34, (s32) arg0, var_s4) == 0) {
            var_s3 |= 1;
        }
        temp_v0_3 = *(s16 *) ((u8 *) arg0 + 0x22);
        temp_v0_3 = (s16) (temp_v0_3 - 1);
        var_s1 = 3;
        if (temp_v0_3 >= 0) {
            var_s1 = temp_v0_3;
        }
        ovl_11_func_800D0408(var_s1, (Recon800D0408A1View *) &sp20, 0x190);
        sp20.vx +=  (*(s32 *) ((u8 *) arg0 + 0x38));
        sp20.vz += *(s32 *) ((u8 *) arg0 + 0x40);
        if ((arg0->unk34 & 0x200000) && (ovl_11_func_800D2DB8((Ovl11Func800BDCE8Arg1 *) &sp20) == 0)) {
            var_s3 |= 4;
        }
        if (arg0->unk34 & 0x800000) {
            if (ovl_11_func_800D24FC(arg0, sp20, (s16) (s32) var_s1, 0) != 1) {
                var_s3 |= 4;
            }
        }
        if (ovl_11_func_800BE208((s32 *) &sp20, *(Ovl11CollisionTag *) ((u8 *) arg0 + 0x2E), temp_s7, temp_s6, &sp30, &sp34, (s32) arg0, var_s4) == 0) {
            var_s3 |= 4;
        }
        temp_v0_3 = *(s16 *) ((u8 *) arg0 + 0x22);
        temp_v0_3 = (s16) (temp_v0_3 + 1);
        var_s1 = 0;
        if (temp_v0_3 < 4) {
            var_s1 = temp_v0_3;
        }
        ovl_11_func_800D0408(var_s1, (Recon800D0408A1View *) (Recon800D0408A1View *) &sp20, 0x190);
        sp20.vx += *(s32 *) ((u8 *) arg0 + 0x38);
        sp20.vz += *(s32 *) ((u8 *) arg0 + 0x40);
        if ((arg0->unk34 & 0x200000) && (ovl_11_func_800D2DB8((Ovl11Func800BDCE8Arg1 *) &sp20) == 0)) {
            var_s3 |= 2;
        }
        if (arg0->unk34 & 0x800000) {
            if (ovl_11_func_800D24FC(arg0, sp20, (s16) (s32) var_s1, 0) != 1) {
                var_s3 |= 2;
            }
        }
        if (ovl_11_func_800BE208((s32 *) &sp20, *(Ovl11CollisionTag *) ((u8 *) arg0 + 0x2E), temp_s7, temp_s6, &sp30, &sp34, (s32) arg0, var_s4) == 0) {
            var_s3 |= 2;
        }
        temp_v0_3 = (s16) ovl_11_func_800D196C(arg0, var_s3, var_s4);
        ovl_11_func_800D0408(temp_v0_3, (Recon800D0408A1View *) ((u8 *) arg0 + 0x68), 0x190);
        if (temp_v0_3 != 4) {
            (*(s16 *) ((u8 *) arg0 + 0x22)) = temp_v0_3;
        }
        (*(s32 *) ((u8 *) arg0 + 0x68)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x68)) + (*(s32 *) ((u8 *) arg0 + 0x38)));
        (*(s32 *) ((u8 *) arg0 + 0x70)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x70)) + (*(s32 *) ((u8 *) arg0 + 0x40)));
        arg0->unk34 |= 0x4000;
        ovl_11_func_800D0408(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
    }
    if ((arg0->unk34 & 0x2000) && (ovl_11_func_800D1CFC(arg0) == 0)) {
        ovl_11_func_800D0408(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
        ovl_11_func_800D0698(arg0);
        return;
    }
    ovl_11_func_800D0408(*(s16 *) ((u8 *) arg0 + 0x22), (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), arg1);
    sp20.vx = (*(s32 *) ((u8 *) arg0 + 0x38)) + (*(s32 *) ((u8 *) arg0 + 0x48));
    sp20.vz = (*(s32 *) ((u8 *) arg0 + 0x40)) + (*(s32 *) ((u8 *) arg0 + 0x50));
    sp20.vy = arg0->unk3C;
    temp_v1_2 = arg0->unk34;
    var_s1 = 0;
    if (!(temp_v1_2 & 0x01000000)) {
        if (temp_v1_2 & 0x200000) {
            var_s1 = ovl_11_func_800D2DB8((Ovl11Func800BDCE8Arg1 *) &sp20) != 0;
        }
        if (ovl_11_func_800BE208((s32 *) &sp20, *(Ovl11CollisionTag *) ((u8 *) arg0 + 0x2E), temp_s7, temp_s6, &sp30, &sp34, (s32) arg0, var_s4) != 0) {
            var_s1 = 1;
        }
        if (arg0->unk34 & 0x800000) {
            if (ovl_11_func_800D24FC(arg0, sp20, (s16) (s32) (*(s16 *) ((u8 *) arg0 + 0x22)), 1) == 1) {
                var_s1 = 1;
            }
        }
        if (var_s1 == 1) {
            ovl_11_func_800D0408(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
            (*(u16 *) ((u8 *) arg0 + 0x2E)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0x2E)) + 2);
        }
        arg0->unk3C = sp20.vy;
        ovl_11_func_800D2B40(arg0);
    } else {
        ovl_11_func_800D2594(arg0, arg1);
    }
    D_80123754 = 0x258;
}
