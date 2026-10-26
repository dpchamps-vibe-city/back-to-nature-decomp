#include "common.h"
#include "game_types.h"
#include "ovl11_area_slot_flags.h"

s32 ovl_11_func_80110188(Ovl11StateArg0 *arg0, s16 arg1, s32 *arg2, s16 arg3);
void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
s32 ovl_11_func_80110544(Ovl11E644Object *arg0, s16 arg1);
s32 ovl_11_func_800F3A18(u16 arg0);
s32 ovl_11_func_800F3BA0(u16 arg0);
s32 ovl_11_func_80110520(s32 arg0, s16 arg1);
void ovl_11_func_80110CE8(s32 *dst, s16 arg1);
void ovl_11_func_80110DA0(s32 *arg0, s16 arg1);
void ovl_11_func_80110E28(u8 *arg0);
void ovl_11_func_800F397C(u16 arg0, u16 arg1);
void ovl_11_func_800F3AF0(u16 arg0, u16 arg1);
s32 ovl_11_func_80110944(s16 arg0, u16 arg1);

s32 ovl_11_func_8010F8E4(Ovl11StateArg0 *arg0, s32 arg1) {
    Ovl11PaddedVec3 sp18;
    s32 selected;
    s32 result;

    switch (*(s16 *)((u8 *)arg0 + 0x28)) {
    case 0:
        if (ovl_11_func_80110944(*(s16 *)((u8 *)arg0 + 0x30), 1) == -1) {
            *(s16 *)((u8 *)arg0 + 0x28) = 0;
            return 1;
        }
        ovl_11_func_80110188(arg0, arg1 == 0, &sp18.field_0, *(s16 *)((u8 *)arg0 + 0x30));
        ovl_11_func_800D05D0((Ovl11SetFieldsView *)arg0, sp18);
        *(s16 *)((u8 *)arg0 + 0x28) = 1;
        *(u16 *)((u8 *)arg0 + 0xB8) |= 0x200;
        break;
    case 1:
        if (ovl_11_func_80110544((Ovl11E644Object *)arg0, 0xE6)) {
            *(s16 *)((u8 *)arg0 + 0x28) = 2;
        }
        break;
    case 2:
        if (arg1 == 1) {
            if (ovl_11_func_800F3A18(1)) {
                *(s16 *)((u8 *)arg0 + 0xB6) = 0x68;
            } else {
                *(s16 *)((u8 *)arg0 + 0x28) = 0;
                return 1;
            }
        } else {
            if (ovl_11_func_800F3BA0(1)) {
                *(s16 *)((u8 *)arg0 + 0xB6) = 0x6B;
            } else {
                *(s16 *)((u8 *)arg0 + 0x28) = 0;
                return 1;
            }
        }
        *(s16 *)((u8 *)arg0 + 0x28) = 3;
        /* fallthrough */
    case 3:
        if (ovl_11_func_80110520((s32)arg0, *(s16 *)((u8 *)arg0 + 0xB6))) {
            selected = ovl_11_func_80110944(*(s16 *)((u8 *)arg0 + 0x30), 1);
            if (selected != -1) {
                *(s16 *)((u8 *)arg0 + 0xBA) = selected;
                if (arg1 == 1) {
                    ovl_11_func_80110CE8(&sp18.field_0, selected);
                } else {
                    ovl_11_func_80110DA0(&sp18.field_0, selected);
                }
                ovl_11_func_800D05D0((Ovl11SetFieldsView *)arg0, sp18);
                *(s16 *)((u8 *)arg0 + 0x28) = 4;
            } else {
                *(s16 *)((u8 *)arg0 + 0x28) = 6;
            }
        }
        break;
    case 4:
        if (ovl_11_func_80110544((Ovl11E644Object *)arg0, 0xE6)) {
            *(s16 *)((u8 *)arg0 + 0x28) = 5;
        }
        break;
    case 5:
        result = Ovl11IsAreaSlotFlagSet(*(s16 *)((u8 *)arg0 + 0xBA), arg1);
        if (result == 1) {
            selected = ovl_11_func_80110944(*(s16 *)((u8 *)arg0 + 0x30), 1);
            if (selected == -1) {
                *(s16 *)((u8 *)arg0 + 0x28) = 7;
            } else {
                *(s16 *)((u8 *)arg0 + 0xBA) = selected;
                if (arg1 == 1) {
                    ovl_11_func_80110CE8(&sp18.field_0, selected);
                } else {
                    ovl_11_func_80110DA0(&sp18.field_0, selected);
                }
                ovl_11_func_800D05D0((Ovl11SetFieldsView *)arg0, sp18);
                *(s16 *)((u8 *)arg0 + 0x28) = 4;
            }
        } else {
            Ovl11SetAreaSlotFlag(*(s16 *)((u8 *)arg0 + 0xBA), arg1);
            ovl_11_func_80110E28((u8 *)arg0);
            *(s16 *)((u8 *)arg0 + 0x28) = 0;
        }
        break;
    case 7:
        if (arg1 == 1) {
            ovl_11_func_80110188(arg0, 0, &sp18.field_0, *(s16 *)((u8 *)arg0 + 0x30));
        } else {
            ovl_11_func_80110188(arg0, 1, &sp18.field_0, *(s16 *)((u8 *)arg0 + 0x30));
        }
        ovl_11_func_800D05D0((Ovl11SetFieldsView *)arg0, sp18);
        *(s16 *)((u8 *)arg0 + 0x28) = 8;
        /* fallthrough */
    case 8:
        if (ovl_11_func_80110544((Ovl11E644Object *)arg0, 0xE6)) {
            *(s16 *)((u8 *)arg0 + 0x28) = 6;
        }
        break;
    case 6:
        if (arg1 == 1) {
            ovl_11_func_800F397C(0, 1);
        } else {
            ovl_11_func_800F3AF0(0, 1);
        }
        ovl_11_func_80110E28((u8 *)arg0);
        *(s16 *)((u8 *)arg0 + 0x28) = 0;
        return 1;
    }
    return 0;
}
