#include "common.h"
#include "game_types.h"

s32 func_8001FABC(s16 arg0);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, u16 arg1, s32 arg2, s32 arg3);
void func_80015840(ObjectState *obj, s8 arg1);
s32 ovl_11_func_800C7270(Recon_ovl_11_func_800C7270_A0View *arg0, s32 arg1, s32 arg2, s32 arg3);
/* 800C6C64 explicitly supplies an unused fourth argument. */
s32 ovl_11_func_800D622C();
void ovl_11_func_800C6A24(Ovl11C7010Object *arg0);
s32 ovl_11_func_800CE37C(Ovl11CE37CArg0 *arg0);

s32 ovl_11_func_800C6AFC(Ovl11C7010Object *arg0) {
    s32 var_v0;
    u16 temp_v0;
    u8 temp_s1;
    u16 held;
    u16 pressed;

    if (!(arg0->unk6C & 0x40)) {
        held = *(u16 *)&D_801231F4;
        pressed = *(u16 *)&D_801231F8;
        temp_v0 = arg0->unk3C;
        switch (temp_v0) {
        case 0x4F:
            if (((*(s32 *) ((u8 *) arg0 + 0x264)) & ~0xFF) == 0x400) {
                func_8001FABC(0xC);
                return 0;
            }
            return 0;
        case 0x58:
            if (pressed & 0x40) {
                arg0->unk6C = (arg0->unk6C | 0x1800) & 0xBFFFFFFF;
                ovl_11_func_800C70CC(arg0, 0x4BU, 0, 0x2D);
                arg0->unk38 = 0;
                func_80015840((ObjectState *) &arg0->sprite, 4);
                (*(s32 *) ((u8 *) arg0 + 0x114)) = -0xFA;
                arg0->sprite.field_5 = (u8) (*(u16 *) ((u8 *) arg0 + 0x38));
                arg0->unk6C = (arg0->unk6C | 0x40000000) & ~1;
                (*(u16 *) ((u8 *) arg0 + 0x36)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0x36)) & 0xFEFF);
                func_8001FABC(6);
            }
            goto block_9;
        case 0x57:
            if (pressed & 0x40) {
                ovl_11_func_800C7270((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x58, 0, D_80122F78 * 0xA);
            }
block_9:
            if (D_8006C844 & 0x100000) {
                ovl_11_func_800D622C(2, -1, 0, 0);
            }
            return 0;
        default:
block_30:
            return 0;
        case 0x3:
        case 0x31:
            if ((u8) arg0->sprite.field_5 >= 2U) {
                arg0->unk6C &= 0xFFFF7FFF;
            }
            return 0;
        case 0x2D:
            if (!(held & 0xF9FF)) {
                if (arg0->unk6C & 0x800000) {
                    if (!(D_80070C88 & 0x1000)) {
                        goto block_18;
                    }
                            return 0;
                }
                goto block_30;
            }
block_18:
            ovl_11_func_800C7270((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x2E, 2, -2);
            arg0->unk6C &= 0xFFBFFFFF;
            return 1;
        case 0x16:
        case 0x18:
        case 0x1A:
        case 0x1C:
        case 0x1E:
            if (held & 0x80) {
                if ((*(s16 *) ((u8 *) arg0 + 0x18)) < ((*(s16 *) ((u8 *) arg0 + 0x86)) * 0x14)) {
                    (*(s16 *) ((u8 *) arg0 + 0x18)) = (s16) ((u16) (*(s16 *) ((u8 *) arg0 + 0x18)) + 1);
                } else {
                    D_80128C6C += 1;
                }
                arg0->sprite.field_5 = (*(s16 *) ((u8 *) arg0 + 0x18)) / 20;
                if ((u32) D_80128C6C >= 0x3DU) {
                    arg0->unk6C |= 0x1800;
                    ovl_11_func_800C70CC(arg0, 0x3EU, 0, -2);
                    D_80128C6C = 0;
                    return 0;
                }
                    return 0;
            }
            ovl_11_func_800C6A24(arg0);
            return 0;
        case 0x4A:
            temp_s1 = arg0->sprite.field_4;
            if (temp_s1 == 1) {
                var_v0 = ovl_11_func_800CE37C((Ovl11CE37CArg0 *) arg0);
                if (var_v0 == temp_s1) {
                    var_v0 = -2;
                } else {
                    var_v0 = 0x84;
                }
                (*(s16 *) ((u8 *) arg0 + 0x1C)) = var_v0;
            }
            return 0;
        }
    } else {
        return 0;
    }
}
