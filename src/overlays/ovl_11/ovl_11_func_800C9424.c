#include "common.h"
#include "game_types.h"
#include "sprite_rotation.h"

s32 ovl_11_func_80112DA8 (Ovl11C7010Object *arg0);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800C93A4 (Ovl11C7010Object *arg0);
s32 ovl_11_func_800C8BA8 (Recon_ovl_11_func_800C7270_A0View *arg0, u16 arg1);
s32 ovl_11_func_800CD734 (Ovl11C7010Object *arg0, s32 arg1);
s32 ovl_11_func_800C907C (Ovl11C8FA8Object *arg0, u16 arg1);
/* This caller supplies a direction; the matched helpers do not use it. */
void ovl_11_func_800C8D9C (Recon_ovl_11_func_800C7270_A0View *arg0, s32 direction);
void ovl_11_func_800C8E98 (Recon_ovl_11_func_800C7270_A0View *arg0, s32 direction);
s32 ovl_11_func_800C7838 (u8 *arg0);
void ovl_11_func_800CF308 (s16 arg0);
/* 8001FAD0 calls func_800212A8 and forwards its return value unchanged. */
s32 func_8001FABC (s16 arg0);
s32 ovl_11_func_800C8D24 (M2C_15512f31231d_Func800C7878Arg0 *arg0, u16 arg1);
s32 ovl_11_func_800C76EC (Ovl11C7010Object *arg0);
s32 ovl_11_func_800C77D8 (Ovl11C7010Object *arg0, u16 arg1);
s32 ovl_11_func_800C97D0 (Recon_ovl_11_func_800C97D0_A0View *arg0);
s32 ovl_11_func_800C99C8 (void);
char *ovl_11_func_8011F574(void);

s32 ovl_11_func_800C9424(void) {
    Ovl11C7010Object *obj;
    u16 pressed;
    u16 held;
    s32 direction;
    u32 delay;

    obj = (Ovl11C7010Object *)&D_80071A00;
    pressed = *(u16 *)&D_801231F8;
    held = *(u16 *)&D_801231F4;
    if ((((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25474 == 0x37 ||
         ((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25474 == 0x43) &&
        ovl_11_func_80112DA8(obj)) {
        return 1;
    }
    if (!(obj->unk6C & 0xA)) {
        if ((obj->unk6C & 0x100) && (pressed & 0x40)) {
            ovl_11_func_800C70CC(obj, 0x3A, 0xB0, -2);
            return 1;
        }
        if (obj->unk6C & 0x40) {
            return 1;
        }
        if (*(u16 *)((u8 *)obj + 0x36) & 0x80) {
            ovl_11_func_800C93A4(obj);
            *(u16 *)((u8 *)obj + 0x36) &= ~0x80;
            return 1;
        }
        if (*(u16 *)((u8 *)obj + 0x36) & 0x20) {
            if (Ovl11GetSpriteRotationState(0x20) == 1) {
                *(u16 *)((u8 *)obj + 0x36) &= ~0x20;
                obj->unk6C |= 0x400;
                ovl_11_func_8011F574();
            }
            return 1;
        }
        if (ovl_11_func_800C8BA8((Recon_ovl_11_func_800C7270_A0View *)obj, pressed)) {
            return 1;
        }
        if (!(obj->unk6C & 0x40000108)) {
            if ((((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x02000000) &&
                ((struct struct_8006C838_time *)D_8006C838)->field_44C0 == 0xC) {
                return 0;
            }
            if (ovl_11_func_800CD734(obj, 0)) {
                return 0;
            }
            if (ovl_11_func_800C907C((Ovl11C8FA8Object *)obj, pressed)) {
                return 1;
            }
            if (pressed & 8) {
                if (obj->itemIndex == 0) {
                    ovl_11_func_800C8D9C((Recon_ovl_11_func_800C7270_A0View *)obj, 0);
                }
                return 2;
            }
            if (pressed & 2) {
                ovl_11_func_800C8E98((Recon_ovl_11_func_800C7270_A0View *)obj, 0);
                return 2;
            }
            if (!(obj->unk6C & 0x18000000)) {
                if (held & 8) {
                    direction = 2;
                    if (pressed & 0x8000) {
                        direction = 1;
                    }
                    if (pressed & 0x2000) {
                        direction = 0;
                    }
                    if (direction != 2) {
                        ovl_11_func_800C8D9C((Recon_ovl_11_func_800C7270_A0View *)obj, direction);
                        return 2;
                    }
                }
                if (held & 2) {
                    direction = 2;
                    if (pressed & 0x8000) {
                        direction = 1;
                    }
                    if (pressed & 0x2000) {
                        direction = 0;
                    }
                    if (direction != 2) {
                        ovl_11_func_800C8E98((Recon_ovl_11_func_800C7270_A0View *)obj, direction);
                        return 2;
                    }
                }
                if (pressed & 4) {
                    ovl_11_func_800C70CC(obj, 0x14, obj->unk38 + 0x43, -2);
                    ovl_11_func_800C7838((u8 *)obj);
                    ovl_11_func_800CF308(1);
                    func_8001FABC(0x12);
                    return 1;
                }
                if (pressed & 1) {
                    ovl_11_func_800C70CC(obj, 0x14, obj->unk38 + 0x43, -2);
                    ovl_11_func_800C7838((u8 *)obj);
                    ovl_11_func_800CF308(2);
                    func_8001FABC(0x12);
                    return 1;
                }
            }
            if (ovl_11_func_800C8D24((M2C_15512f31231d_Func800C7878Arg0 *)obj, pressed)) {
                return 1;
            }
            if (ovl_11_func_800C76EC(obj)) {
                return 0;
            }
            if (ovl_11_func_800C77D8(obj, pressed)) {
                return 1;
            }
            if (ovl_11_func_800C97D0((Recon_ovl_11_func_800C97D0_A0View *)obj)) {
                return 1;
            }
        }
    }
    if (!(obj->unk6C & 0x40000000)) {
        if (ovl_11_func_800C99C8()) {
            delay = 0xC;
            if (obj->unk3C == 4) {
                delay = 8;
            }
            if (delay < D_80123150) {
                D_80128C70 = func_8001FABC(0x48);
                D_80123150 = 0;
            } else {
                D_80123150++;
            }
            return 1;
        }
    }
    return 0;
}
