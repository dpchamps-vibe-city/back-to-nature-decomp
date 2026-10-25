#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "screen_fade.h"
#include "pad_actuator_sequences.h"

void ovl_11_func_800CD08C (Recon_ovl_11_func_800CD08C_A0View *arg0);
s32 func_8001FABC (s16 arg0);
s32 ovl_11_func_800CD0F4 (Recon_ovl_11_func_800CCFF8_A0View *arg0, s32 arg1);
s32 ovl_11_func_800D58D0 (void *arg0);
void ovl_11_func_800CCF14 (C580C_7f0a9cc905ed_UnkStruct800D72E8 *arg0);
s32 ovl_11_func_800CB4E0 (void);
void ovl_11_func_800CF308 (s16 arg0);
s32 ovl_11_func_800CD3C4 (s32 arg0);
s32 ovl_11_func_800D622C (s8 arg0, s8 arg1, s16 arg2, s32 arg3);
void ovl_11_func_800C4A2C (Ovl11C4A2CObject *arg0);
void ovl_11_func_800C6770 (Ovl11C6770Object *arg0);
void ovl_11_func_800C70CC (Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800C7270 (Recon_ovl_11_func_800C7270_A0View *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8001AF70 (u16 arg0, u16 arg1);
s32 ovl_11_func_80107C80 (s16 arg0, u16 arg1, s32 *arg2, s32 arg3);
void ovl_11_func_800CD670 (s16 arg0);
void func_8001FE34 (s32 arg0);
s32 ovl_11_func_800F3E44 (s16 arg0);
s32 func_8002261C (s32 arg0, s32 arg1);
void ovl_11_func_800CBD78 (Ovl11C7010Object *arg0);
s32 ovl_11_func_800CF044 (s16 arg0, Ovl25SetupStatePair *arg1, VECTOR *arg2);
void ovl_11_func_800CF314 (u8 *arg0, C580C_b6c4596908ca_Ovl11CF314Arg1 *arg1, C580C_b6c4596908ca_Ovl11CF314Arg2 *arg2);
s32 ovl_11_func_800CDF4C (u8 *arg0);
void ovl_11_func_800C5738 (Ovl11C498CObject *arg0);
void ovl_11_func_800CE034 (C580C_ad72053e5aa9_StructOvl11CE034A *arg0, C580C_ad72053e5aa9_StructOvl11CE034B *arg1, s32 arg2);
void ovl_11_func_800F43CC(s32 arg0, VECTOR position, u16 arg5, s32 arg6);
void ovl_11_func_800F0FB0 (s32 arg0);
void ovl_11_func_800D666C (s16 arg0);
void ovl_11_func_800D7328 (s16 *arg0);
s32 ovl_11_func_800CB540 (u16 *arg0);
void ovl_11_func_800F3AF0 (u16 arg0, u16 arg1);
s32 ovl_11_func_800F2354 (s32 arg0, s32 arg1);
s32 ovl_11_func_800C8FA8 (Ovl11C8FA8Object *arg0);
s32 ovl_11_func_8010CE80 (Recon_ovl_11_func_8010CE80_A0View *arg0, s32 arg1);
C580C_f3fe4d9631b5_UnkStruct800CBF40 *ovl_11_func_800CBF70 (C580C_f3fe4d9631b5_UnkStruct800CBF70Arg0 *arg0, u16 *arg1, s16 arg2);
void ovl_11_func_800C55FC (Ovl11CE034Object *arg0);
s32 ovl_11_func_800CE53C (s16 arg0);
void ovl_11_func_800D7348 (u16 *dst, u16 *src);
void ovl_11_func_800C9D98 (void);
s32 ovl_11_func_800C6858 (Ovl11C7010Object *arg0);
s32 ovl_11_func_800CE514 (s16 arg0);
void ovl_11_func_800CCF58 (Ovl11C7010Object *arg0);
s32 ovl_11_func_800C4DE0 (Ovl11C7010Object *arg0);
s32 ovl_11_func_80113310 (Ovl11CE034Object *arg0);
void ovl_11_func_800CD5BC (C580C_583b0b68ff5a_UnkStruct800CD5BC *arg0, s32 arg1);
s32 ovl_11_func_800C4B6C (Ovl11C7010Object *arg0);
s32 ovl_11_func_800C4C94 (Ovl11C7010Object *arg0);
void func_8001FAE8 (s32 arg0);
s32 ovl_11_func_800C5098 (Ovl11CD5BCObject *arg0);
void ovl_11_func_800C541C (Ovl11CD5BCObject *arg0);
void ovl_11_func_800C523C (Ovl11CD5BCObject *arg0);
s32 ovl_11_func_800F3EF0(void);

s32 ovl_11_func_800C580C(Ovl11C580CObject *arg0) {
    s16 column;
    VECTOR position;
    Ovl11CC7CCRequest request1;
    Ovl11CC7CCRequest request2;
    Ovl11CC7CCRequest request3;
    C580C_7f0a9cc905ed_UnkStruct800D72E8 *temp_s0;
    C580C_7f0a9cc905ed_UnkStruct800D72E8 *temp_s0_2;
    C580C_7f0a9cc905ed_UnkStruct800D72E8 *temp_s0_3;
    s16 temp_s0_4;
    s16 temp_s0_5;
    s16 temp_v1_4;
    s16 var_a1_3;
    s16 var_a2_2;
    s32 var_s0;
    s32 applyCost;
    s32 waitForAction;
    s32 var_v0;
    u16 *temp_s0_6;
    u16 *var_a0_3;
    u16 temp_v0;
    s32 temp_v0_2;
    s32 status;
    u8 temp_v1;
    void *temp_a0;
    void *temp_v0_3;
    void *temp_v1_3;

    waitForAction = 0;
    if (arg0->unk6C & 0x1000) {
        temp_v0 = arg0->state;
        switch (temp_v0) {
        case 0x3D:
        case 0x3E:
        case 0x3F:
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
            arg0->unk6C &= ~0x4000;
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
reset_done:
            return 0;
        case 0x52:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            func_8001FABC(0x15);
            arg0->timer = 0x64;
            goto negative_action;
        case 0x55:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            func_8001FABC(0x11);
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 8);
            temp_s0 = (C580C_7f0a9cc905ed_UnkStruct800D72E8 *) ((u8 *) arg0 + 0x84);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0(temp_s0);
            }
            ovl_11_func_800CCF14(temp_s0);
            arg0->unk6C &= 0xFFFF7FFB;
            ovl_11_func_800CB4E0();
            ovl_11_func_800D622C(0, ovl_11_func_800CD3C4(0), 0, 0);
            return 0;
        case 0x50:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            ovl_11_func_800CF308(0);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0((void *) ((u8 *) arg0 + 0x84));
            }
            ovl_11_func_800CB4E0();
            ovl_11_func_800D622C(0, ovl_11_func_800CD3C4(0), 0, 0);
            return 0;
        case 0x51:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            func_8001FABC(0x11);
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 4);
            temp_s0_2 = (C580C_7f0a9cc905ed_UnkStruct800D72E8 *) ((u8 *) arg0 + 0x84);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0(temp_s0_2);
            }
            ovl_11_func_800CCF14(temp_s0_2);
            arg0->unk6C &= 0xFFFF7FFB;
            ovl_11_func_800CB4E0();
            ovl_11_func_800D622C(0, ovl_11_func_800CD3C4(0), 0, 0);
            return 0;
        case 0x4E:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 6);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0((void *) ((u8 *) arg0 + 0x84));
            }
            arg0->unk6C &= ~4;
            ovl_11_func_800D622C(0, ovl_11_func_800CD3C4(0), 0, 0);
            return 0;
        case 0x4F:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 2);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0((void *) ((u8 *) arg0 + 0x84));
            }
            ovl_11_func_800CB4E0();
            ovl_11_func_800D622C(0, ovl_11_func_800CD3C4(0), 0, 0);
            return 0;
        case 0x4D:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 5);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0((void *) ((u8 *) arg0 + 0x84));
            }
            arg0->unk6C &= ~4;
            ovl_11_func_800D622C(0, ovl_11_func_800CD3C4(0), 0, 0);
            return 0;

        case 0x53:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            func_8001FABC(0xE);
            ovl_11_func_800C4A2C((Ovl11C4A2CObject *) arg0);
            status = arg0->unk6C;
            status &= 0xEFFFFFFF;
            arg0->unk6C = status;
            ovl_11_func_800CCF14((C580C_7f0a9cc905ed_UnkStruct800D72E8 *) ((u8 *) arg0 + 0x84));
            ovl_11_func_800CB4E0();
negative_action:
            ovl_11_func_800D622C(-1, ovl_11_func_800CD3C4(0), 0, 0);
            break;
        case 0x54:
            ovl_11_func_800C7270((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x2D, 1, -2);
            return 0;
        case 0x4C:
            arg0->state = 0x31U;
            ovl_11_func_800C6770((Ovl11C6770Object *) arg0);
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            arg0->unk6C &= ~0x4000;
            ovl_11_func_800CB4E0();
            return 0;
        case 0x3B:
            func_8001FABC(0x10);
            arg0->timer114 = 0;
            arg0->direction = 0U;
            arg0->unk6C |= 1;
            ovl_11_func_800C70CC((Ovl11C7010Object *) arg0, 0x56U, 0, -2);
state_done:
            return 0;
        case 0x56:
            arg0->unk6C |= 0x1800;
            arg0->attributes |= 0x100;
            arg0->unk6C &= ~0x40000000;
            ovl_11_func_800C70CC((Ovl11C7010Object *) arg0, 0x3CU, 0, D_80122F78 * 0xA);
            arg0->unk6C |= 0x40000000;
            ovl_11_func_800C7270((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x58, 0, D_80122F78 * 0xA);
            RequestPadActuatorSequence(0, 3);
            return 0;
        case 0x58:
            ovl_11_func_800C7270((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x57, 1, D_80122F78 * 0xA);
            return 0;
        case 0x45:
            arg0->unk6C = (arg0->unk6C | 0x10) & 0xDFFFFFFF;
            goto state_done;
        case 0x49:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            temp_s0_3 = (C580C_7f0a9cc905ed_UnkStruct800D72E8 *) ((u8 *) arg0 + 0x8A);
            arg0->unk6C &= 0xEFFF3FFF;
            ovl_11_func_800D58D0(temp_s0_3);
            ovl_11_func_800CCF14(temp_s0_3);
            ovl_11_func_800CB4E0();
            return 0;
        case 0x46:
            if ((D_80128C50 & 0x80) && (ovl_11_func_800F3EF0() != 0)) {
                func_8001FABC(0x13);
                func_8001AF70(7U, 1U);
            }
            arg0->unk6C &= ~0x4000;
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            ovl_11_func_800CB4E0();
            return 0;
        case 0x4B:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            ovl_11_func_80107C80(0x2F, 0U, NULL, 1);
            arg0->direction = (u16) ((arg0->direction + 2) & 3);
            ovl_11_func_800CD670(0x14);
            return 0;
        case 0x4A:
            temp_v1 = arg0->phase;
            if (temp_v1 == 3) {
                ((struct Ovl11C580CWork *)&D_8007AFF0)->mode = 0xC;
                BeginScreenFadeIncrease(0x1E, 0, 2);
                func_8001FE34(0x1E);
                arg0->unk6C |= 0x400;
                func_8001AF70(0x47U, 1U);
                return 0;
            }
            if (temp_v1 == 1) {
                func_8001AF70(0xAU, 1U);
            }
            ovl_11_func_800C7270((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x4A, arg0->phase + 1, *((s32 *) ((u8 *) &D_80123244 + (arg0->phase * 4))));
            return 0;

        default:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            goto reset_done;
        }
        return 0;
    } else {
        status = arg0->state;
        applyCost = 0;
        switch (status) {
        case 0x6F:
            func_8001FABC(9);
            ((struct Ovl11C580CState *)D_8006C838)->record->flags |= 0x100;
            D_80129634 = 3;
            if (ovl_11_func_800F3E44(arg0->item) != 0) {
                func_8002261C(3, 0x383);
            }
            arg0->unk6C &= 0xE7FF3FFF;
            ovl_11_func_800CBD78((Ovl11C7010Object *) arg0);
            ovl_11_func_800CB4E0();
            return 0;
        case 0x3A:
            if (arg0->unk6C & 0x80) {
                ovl_11_func_800CF044(*(s16 *)Ovl11SavedInteractionObject, (Ovl25SetupStatePair *) ((u8 *) arg0 + 0xFC), (VECTOR *) ((u8 *) arg0 + 0x100));
            } else {
                ovl_11_func_800CF314((u8 *)&D_80075AD4, (C580C_b6c4596908ca_Ovl11CF314Arg1 *) ((u8 *) arg0 + 0xFC), (C580C_b6c4596908ca_Ovl11CF314Arg2 *) ((u8 *) arg0 + 0x100));
            }
            arg0->unk6C &= ~0x180;
            ovl_11_func_800CDF4C((u8 *) arg0);
            ovl_11_func_800C5738((Ovl11C498CObject *) arg0);
            return 1;
        case 0x59:
        case 0x5A:
        case 0x5B:
        case 0x5C:
        case 0x5D:
        case 0x5E:
        case 0x5F:
        case 0x61:
        case 0x62:
        case 0x64:
        case 0x65:
        case 0x66:
        case 0x69:
        case 0x6A:
        case 0x6B:
        case 0x6E:
            temp_s0_4 = arg0->item;
            if (arg0->state == 0x61) {
                func_8001FABC(0x13);
            }
            if (arg0->state == 0x62) {
                ovl_11_func_800CE034((C580C_ad72053e5aa9_StructOvl11CE034A *) arg0, (C580C_ad72053e5aa9_StructOvl11CE034B *)&position, 4);
                ovl_11_func_800F43CC(0x64, position, arg0->collisionId, 0);
                func_8001FABC(7);
            }
            if (arg0->state == 0x6E) {
                func_8001FABC(0x10);
                ((struct Ovl11C580CState *)D_8006C838)->selected = (u16) arg0->item;
                if (arg0->item == 0xA4) {
                    ((struct Ovl11C580CState *)D_8006C838)->selected = (u16) ((struct Ovl11C580CState *)D_8006C838)->fallback;
                }
                ovl_11_func_800F0FB0(4);
            }
            arg0->unk6C &= 0xEFFF3FFF;
            ovl_11_func_800D666C(arg0->item);
            ovl_11_func_800D7328((s16 *) ((u8 *) arg0 + 0x8A));
            if (arg0->state == 0x6A) {
                func_8001AF70(0x45U, 1U);
                temp_v0_2 = ((struct Ovl11C580CState *)D_8006C838)->count + 1;
                ((struct Ovl11C580CState *)D_8006C838)->count = temp_v0_2;
                if ((s16) temp_v0_2 < 2) {
                    ovl_11_func_800D622C(0, -5, 0, 0);
                }
            }
            if (arg0->state == 0x59) {
                request1.id = 0x8E;
                if (temp_s0_4 == 0x8C) {
                    request1.id = 0x8F;
                }
                if (temp_s0_4 == 0x8D) {
                    request1.id = 0x90;
                }
                if (temp_s0_4 == 0xE3) {
                    request1.id = 0xE4;
                }
                request1.count = 1;
                func_8001FABC(7);
                ovl_11_func_800CB540(&request1.id);
                return 0;
            } else if (arg0->state == 0x5A) {
                request2.id = 0x94;
                if (temp_s0_4 == 0x92) {
                    request2.id = 0x95;
                }
                if (temp_s0_4 == 0x93) {
                    request2.id = 0x96;
                }
                if (temp_s0_4 == 0xE7) {
                    request2.id = 0xE6;
                }
                request2.count = 1;
                func_8001FABC(7);
                ovl_11_func_800CB540(&request2.id);
                return 0;
            } else if (arg0->state == 0x5B) {
                request3.id = 0x88;
                if (temp_s0_4 == 0x123) {
                    request3.id = 0x89;
                }
                if (temp_s0_4 == 0x124) {
                    request3.id = 0x8A;
                }
                if (temp_s0_4 == 0xE8) {
                    request3.id = 0xE5;
                }
                if (temp_s0_4 == 0x125) {
                    request3.id = 0x8A;
                }
                request3.count = 1;
                func_8001FABC(7);
                ovl_11_func_800CB540(&request3.id);
                return 0;
            } else if (arg0->state == 0x5C) {
                ovl_11_func_800F3AF0(1U, 0U);
            }
            ovl_11_func_800CB4E0();
            break;
        case 0x33:
            ovl_11_func_800F2354((s32) (*(s16 *) ((u8 *) D_8006C858 + 0x1002)), 0);
            ovl_11_func_800D666C(arg0->item);
            ovl_11_func_800D7328((s16 *) ((u8 *) arg0 + 0x8A));
            arg0->unk6C &= 0xEFFFFFFF;
            ovl_11_func_800CB4E0();
            goto advance_action;
        case 0x34:
            arg0->kind = 1;
            if (arg0->item == 0x6E) {
                arg0->kind = 2;
            }
            arg0->slot = (u16) *((u16 *) ((u8 *) &D_8012323C + (arg0->kind * 2)));
            ovl_11_func_800D666C(arg0->item);
            ovl_11_func_800D7328((s16 *) ((u8 *) arg0 + 0x8A));
            arg0->unk6C &= 0xEFFFFFFF;
            ovl_11_func_800CB4E0();
            goto advance_action;
        case 0x45:
            arg0->unk6C |= 0x10;
        default:
finish_action:
            goto advance_action;
        case 0x32:
            if ((ovl_11_func_800C8FA8((Ovl11C8FA8Object *) arg0) == 0) && (D_80128C54 & 0x400)) {
                ovl_11_func_8010CE80((Recon_ovl_11_func_8010CE80_A0View *) D_80128C4C_dispatch, 0xD);
                ovl_11_func_800CB4E0();
            }
            ovl_11_func_800D7328((s16 *) ((u8 *) arg0 + 0xF0));
            arg0->unk6C &= 0xFFFF7FFF;
            goto finish_action;
        case 0xA:
            arg0->unk6C |= 0x10000000;
            /* fallthrough */
        case 0x15:
            if (arg0->state == 0x15) {
                arg0->unk6C &= 0xFFFF7FFF;
            }
            ovl_11_func_800D7328((s16 *) ((u8 *) arg0 + 0xF0));
            ovl_11_func_800CB4E0();
            return 0;
        case 0x2F:
            func_8001FABC(9);
            ovl_11_func_800CBF70((C580C_f3fe4d9631b5_UnkStruct800CBF70Arg0 *) ((u8 *) arg0 + 0x100), (u16 *) ((u8 *) arg0 + 0x8A), (s16) arg0->direction);
            arg0->unk6C |= 0x40;
            /* fallthrough */
        case 0x9:
            if (arg0->state == 9) {
                func_8001FABC(9);
            }
            arg0->unk6C &= 0xE7FF3FFF;
            if (arg0->state == 9) {
                ovl_11_func_800CBD78((Ovl11C7010Object *) arg0);
            }
            ovl_11_func_800CB4E0();
            return 0;
        case 0x38:
            func_8001FABC(9);
            ovl_11_func_800C55FC((Ovl11CE034Object *) arg0);
            arg0->unk6C &= 0xE7FF3FFF;
            ovl_11_func_800CBD78((Ovl11C7010Object *) arg0);
            ovl_11_func_800CB4E0();
            return 0;
        case 0xC:
            if (ovl_11_func_800CE53C(arg0->item) == 0) {
                temp_s0_6 = (u16 *) ((u8 *) arg0 + 0x8A);
                ovl_11_func_800D7348((u16 *) ((u8 *) arg0 + (((s16) arg0->slot * 6) + 0xBA)), temp_s0_6);
                ovl_11_func_800C9D98();
                var_a0_3 = temp_s0_6;
            } else {
                ovl_11_func_800D666C(arg0->item);
                var_a0_3 = (u16 *) ((u8 *) arg0 + 0x8A);
            }
            ovl_11_func_800D7328((s16 *) var_a0_3);
            arg0->unk6C &= 0xEFFFFFFF;
            ovl_11_func_800CB4E0();
            return 0;
        case 0xD:
            arg0->unk6C |= 0x10000000;
            /* fallthrough */
        case 0x6:
        case 0xE:
        case 0x30:
            ovl_11_func_800C6858((Ovl11C7010Object *) arg0);
            if (ovl_11_func_800CE514(arg0->item) == 1) {
                ovl_11_func_800C70CC((Ovl11C7010Object *) arg0, 0x2FU, arg0->direction + 0x17, -2);
            }
            if (ovl_11_func_800CE53C(arg0->item) != 1) {
                goto tool_done;
            }
            arg0->unk6C |= 0x10000000;
            ovl_11_func_800C70CC((Ovl11C7010Object *) &D_80071A00, 0x67U, 0x1B, 0x1E);
tool_done:
            return 0;
        case 0x67:
            arg0->unk6C |= 0x10000000;
            ovl_11_func_800C70CC((Ovl11C7010Object *) &D_80071A00, 0xCU, (*(u16 *) ((u8 *) &D_80071A00 + 0x38)) + 0x1D, -2);
            arg0->unk6C &= 0xFFFF7FFF;
            return 0;
        case 0x2C:
            ovl_11_func_800C70CC((Ovl11C7010Object *) arg0, 0x2D, 0xB1, -2);
            return 0;
        case 0x3:
        case 0x31:
            arg0->unk6C &= ~0x4000;
            ovl_11_func_800C6770((Ovl11C6770Object *) arg0);
            ovl_11_func_800CCF58((Ovl11C7010Object *) arg0);
            ovl_11_func_800CB4E0();
            goto advance_action;
        case 0x28:
            ovl_11_func_800CCF14((C580C_7f0a9cc905ed_UnkStruct800D72E8 *) ((u8 *) arg0 + 0x8A));
            ovl_11_func_800CB4E0();
            goto advance_action;
        case 0x17:
            if (ovl_11_func_800C4DE0((Ovl11C7010Object *) arg0) == 1) {
                applyCost = 0;
                waitForAction = 1;
            } else {
                applyCost = 1;
            }
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            goto advance_action;
        case 0x19:
            temp_v1_4 = *((struct Ovl11C580CWork *)&D_8007AFF0)->header;
            if ((temp_v1_4 == 0x37) || (temp_v1_4 == 0x43)) {
                var_s0 = 0x28;
                waitForAction = ovl_11_func_80113310((Ovl11CE034Object *) arg0);
                if (waitForAction != 0) {
                    var_s0 = 0x96;
                }
                ovl_11_func_800CD5BC((C580C_583b0b68ff5a_UnkStruct800CD5BC *) arg0, var_s0);
                applyCost = 1;
            } else {
                if (ovl_11_func_800C4B6C((Ovl11C7010Object *) arg0) == 1) {
                    applyCost = 0;
                    waitForAction = 1;
                } else {
                    applyCost = 1;
                }
            }
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            RequestPadActuatorSequence(0, 2);
            goto advance_action;
        case 0x1F:
            if (ovl_11_func_800C4C94((Ovl11C7010Object *) arg0) == 1) {
                applyCost = 0;
                waitForAction = 1;
            } else {
                applyCost = 1;
            }
            func_8001FAE8((s32) D_80128C70);
            D_80128C70 = -1;
            goto finish_action;
        case 0x68:
            if (ovl_11_func_800C5098((Ovl11CD5BCObject *) arg0) != 1) {
                applyCost = 0;
                waitForAction = 1;
            } else {
                applyCost = 1;
            }
            goto finish_action;
        case 0x1D:
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            ovl_11_func_800C541C((Ovl11CD5BCObject *) arg0);
            RequestPadActuatorSequence(0, 2);
            applyCost = 1;
            goto finish_action;
        case 0x1B:
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            ovl_11_func_800C523C((Ovl11CD5BCObject *) arg0);
            RequestPadActuatorSequence(0, 2);
            applyCost = 1;
            goto finish_action;
        case 0x39:
            return 1;
        }

advance_action:
        if (applyCost == 1) {
            if (waitForAction == 0) {
                ovl_11_func_800CB4E0();
            }
            column = (s16)arg0->power / 20;
            temp_s0_5 = D_801231C0[(s16)((s16)(arg0->value - 1) / 5)][column];
            ovl_11_func_800D622C((s8) temp_s0_5, ovl_11_func_800CD3C4(0), 0, 0);
        }
        var_v0 = 0;
        if (waitForAction == 1) {
            return var_v0;
        }
        {
            temp_v0_3 = *(void **) ((u8 *) arg0 + 0x4C);
            temp_a0 = (void *) ((u8 *) temp_v0_3 + 6);
            if (temp_v0_3 != NULL) {
                (*(void **) ((u8 *) arg0 + 0x4C)) = temp_a0;
                if ((*(s16 *)((u8 *)temp_v0_3 + 6)) == 0x270F) {
                    ovl_11_func_800CB4E0();
                    (*(void **) ((u8 *) arg0 + 0x4C)) = NULL;
                    goto script_finished;
                }
                if ((*(s16 *)((u8 *)temp_a0 + 4)) != 0) {
                    arg0->unk6C |= 0x1800;
                }
                temp_v1_3 = *(void **) ((u8 *) arg0 + 0x4C);
                arg0->attributes = (u16) (arg0->attributes & 0xFFEF);
                var_a1_3 = (*(s16 *)temp_v1_3);
                var_a2_2 = (*(s16 *)((u8 *)temp_v1_3 + 2));
                ovl_11_func_800C70CC((Ovl11C7010Object *) arg0, var_a1_3, (s32) var_a2_2, -2);
                return 0;
            }
script_finished:
            var_v0 = 1;
            return var_v0;
        }
    }
}
