#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800CB730(Ovl11CE034Object *arg0);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);
/* This caller zero-extends the byte; the callee stores only its low byte. */
void func_80015840(ObjectState *obj, u8 arg1);
s32 ovl_11_func_800D736C(s16 *arg0);
s32 func_8001FABC(s16 arg0);

s32 ovl_11_func_800CB850(s32 arg0) {
    Ovl11C7010Object *obj;
    ItemData *item;
    s32 result;
    u16 allow;
    s32 itemStatus;
    u16 flags;

    obj = (Ovl11C7010Object *)&D_80071A00;
    if (arg0 == 0) {
        result = ovl_11_func_800CB730((Ovl11CE034Object *)obj);
        item = &(*(ItemData **)((u8 *)obj - 0x51A8))[obj->itemIndex];
        if (obj->itemIndex == 0x65) {
            if (D_80128C54 != 0 && result != 0) {
                return 0;
            }
            goto common_action;
        } else if (obj->itemIndex == 0x64) {
            if (D_80128C50 & 0xC087) {
                return 0;
            }
            if (D_80128C54 != 0) {
                return 0;
            }
            ovl_11_func_800C70CC(obj, 0x62, obj->unk38 + 0x17, -2);
            return 1;
        } else if (obj->itemIndex == 0x84) {
            if (D_80128C54 != 0 && result != 0) {
                return 0;
            }
            goto common_action;
        } else {
            flags = item->u0.flags;
            if (flags & 0x100) {
                if (D_80128C50 & 0xC087) {
                    return 0;
                }
                if (D_80128C54 == 0 || result == 0) {
                    ovl_11_func_800C70CC(obj, 9, obj->unk38 + 0x17, -2);
                    return 1;
                }
            }
            flags = item->u0.flags;
            if ((flags & 0x9000) == 0x9000 || (itemStatus = *(s32 *)((u8 *)item + 0x14), itemStatus == -1)) {
                allow = 0;
                if (D_80128C54 == 0) {
                    allow = 1;
                } else {
                    if (D_80128C54 & 0x8000) {
                        if (((u16 *)D_80128C4C_dispatch)[1] == 0x64 && item->field_12 != 0) {
                            ovl_11_func_800C70CC(obj, 0x6F, obj->unk38 + 0x17, -2);
                            return 1;
                        }
                        allow = 1;
                    }
                    if (D_80128C54 & 0x200) {
                        allow = ((Ovl11CB850State *)D_8006C838)->selectedRecord == 0x15F;
                    }
                }
                if (allow) {
                    goto common_action;
                }
                itemStatus = *(s32 *)((u8 *)item + 0x14);
            }
            if (itemStatus & 0x10) {
                if (result == 0 && D_80128C54 == 0) {
                    ovl_11_func_800C70CC(obj, 0x38, obj->unk38 + 0x17, -2);
                    return 1;
                }
                if (result == 3 && D_80128C54 == 0) {
                    goto common_action;
                }
                if (result == 2) {
                    return 1;
                }
            }
            if (D_80128C54 != 0 || result != 2) {
                return 0;
            }
        }
common_action:
        ovl_11_func_800C70CC(obj, 0x2F, obj->unk38 + 0x17, -2);
        return 1;
    }
    if (obj->itemIndex == 0x64) {
        obj->unk6C |= 0x1800;
        ovl_11_func_800C70CC(obj, 0x46, 0, -2);
        func_80015840((ObjectState *)&obj->sprite, (u8)obj->unk38);
        obj->unk6C |= 0x4000;
        return 1;
    }
    switch ((u32)ovl_11_func_800D736C(&obj->itemIndex)) {
    case 1:
        return 1;
    case 3:
        ovl_11_func_800C70CC(obj, 3, 5, -2);
        func_8001FABC(0x4A);
        return 1;
    case 4:
        obj->unk6C |= 0x5800;
        ovl_11_func_800C70CC(obj, 0x49, 0, -2);
        func_8001FABC(0x4B);
        return 1;
    }
    return 0;
}
