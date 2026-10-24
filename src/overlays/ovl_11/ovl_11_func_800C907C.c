#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800C8FA8 (Ovl11C8FA8Object *arg0);
s32 ovl_11_func_8010CE80 (Recon_ovl_11_func_8010CE80_A0View *arg0, s32 arg1);
s32 ovl_11_func_800CB4E0 (void);
s32 func_8001FABC (s16 arg0);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_8010B57C (u16 *arg0, s32 arg1);
s32 ovl_11_func_800CEEF8 (Ovl11InteractionRecord *arg0);
s16 ovl_11_func_800F4240(Ovl11InteractionRecord *arg0);
s32 func_8001AF44 (u32 arg0);
s32 func_8002261C (s32 arg0, s32 arg1);
s32 ovl_11_func_800CD0F4 (Recon_ovl_11_func_800CCFF8_A0View *arg0, s32 arg1);

s32 ovl_11_func_800C907C(Ovl11C8FA8Object *arg0, u16 arg1) {
    s32 var_s0;
    s32 var_v1;
    u16 temp_v0;
    u16 temp_v1;

    if (arg1 & 0x40) {
        if (ovl_11_func_800C8FA8(arg0) != 0) {
            return 1;
        }
        if (D_80128C54 & 0x400) {
            if (!(D_80070C88 & 2)) {
                ovl_11_func_8010CE80(D_80128C4C_dispatch, 6);
                ovl_11_func_800CB4E0();
                return 1;
            }
            arg0->unk22 = *(u16 *) ((u8 *) D_80128C4C_dispatch + 0xAC);
            ovl_11_func_800CB4E0();
        }
        if (D_80128C54 & 0x200) {
            if (!(arg0->unk6C & 0x18000000)) {
                temp_v1 = *(u16 *) ((u8 *) D_80128C4C_dispatch + 0);
                if (temp_v1 == 0x15F) {
                    ((struct Ovl11C907CStateView *) D_8006C838)->field_92D0 |= 0x20;
                    func_8001FABC(6);
                    arg0->unk38 = ((struct Ovl11C907CStateView *) D_8006C838)->field_92BE;
                    ovl_11_func_800C70CC((Ovl11C7010Object *) arg0, 0x39U, arg0->unk38 + 0x32, -2);
                    arg0->unk6C |= 0x100;
                } else if (temp_v1 == 0x15E) {
                    ovl_11_func_8010B57C(&D_80075AD4, 5);
                }
                return 1;
            }
            return (*(u16 *) ((u8 *) D_80128C4C_dispatch + 0)) != 0x15F;
        }
        if ((D_80128C54 & 2) && (arg0->unk34 != 0)) {
            Ovl11SavedInteractionObject = D_80128C4C_dispatch;
            func_8001FABC(6);
            temp_v0 = *(u16 *) ((u8 *) Ovl11SavedInteractionObject + 0x22);
            arg0->unk38 = temp_v0;
            ovl_11_func_800C70CC((Ovl11C7010Object *) arg0, 0x39U, arg0->unk38 + 0x32, -2);
            arg0->unk6C = (arg0->unk6C | 0x180) & 0xFFDDFFFF;
            ovl_11_func_800CEEF8((Ovl11InteractionRecord *) Ovl11SavedInteractionObject);
            return 1;
        }
        var_s0 = -1;
        if (D_80128C54 & 0x8020) {
            var_v1 = -1;
            if (D_80128C54 & 0x8000) {
                var_v1 = ovl_11_func_800F4240((Ovl11InteractionRecord *) D_80128C4C_dispatch);
            }
            if (D_80128C54 & 0x20) {
                var_v1 = (s32) (s16) (*(u16 *) ((u8 *) D_80128C4C_dispatch + 0));
            }
            switch (var_v1) {
            case 0x3B:
            case 0x16C:
            case 0x16D:
            case 0x16E:
                var_s0 = 0x25F;
                break;
            case 0x3C:
            case 0x16F:
            case 0x170:
            case 0x171:
                var_s0 = 0x260;
                break;
            case 0x3D:
            case 0x172:
            case 0x173:
            case 0x174:
                if (((struct Ovl11D2764StateView *) &D_8007AFF0)->field_25476 == 0xE) {
                    if (func_8001AF44(0xD7U) == 1) {
                        var_s0 = 0x283;
                    } else {
                        var_s0 = 0x282;
                    }
                } else {
                    var_s0 = 0x25E;
                }
                break;
            }
            if (var_s0 != -1) {
                func_8002261C(2, var_s0);
                return 1;
            }
        }
        ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 1);
    }
    return 0;
}
