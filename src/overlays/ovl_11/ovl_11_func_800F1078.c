#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "ovl11_condition_masks.h"

s32 ovl_11_func_800F19E0(u8 arg0, u8 arg1);
s32 ovl_11_func_800F14D8(s32 arg0, s32 arg1, s32 arg2);
s32 ovl_11_func_800F144C(u16 arg0, u16 arg1, s16 *arg2);
s32 ovl_11_func_800F1540(u16 arg0, s32 arg1);
s32 ovl_11_func_800F15B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 ovl_11_func_800F1678(u8 arg0);
s32 ovl_11_func_800F1744(u8 arg0, u16 arg1, s32 arg2);
s32 ovl_11_func_800F1878(u32 arg0, s32 arg1);
s32 ovl_11_func_800F1954(s32 arg0);
s32 ovl_11_func_800F1AE0(s32 arg0);
s32 ovl_11_func_800F1AA8(u8 arg0);
s32 ovl_11_func_800F12D0(s32 arg0, s32 arg1);
/* 800F1D54 defines a2 in every switch arm and fills six halfwords through a1. */
void ovl_11_func_800F1D54(u32 arg0, u16 *arg1);

s32 ovl_11_func_800F1078(Ovl11ConditionRecord *arg0, s32 arg1) {
    u16 bounds[6];
    s32 i;

    for (i = 0; i < arg1; i++, arg0++) {
        ((struct Ovl11ConditionState *)D_8006C838)->current = arg0;
        if (((arg0->unk22 != 2) ||
             !(((struct Ovl11ConditionObjects *)&D_8007AFF0)->second->flags & 0x1000) ||
             !(((struct Ovl11ConditionObjects *)&D_8007AFF0)->first->flags & 0x1000) ||
             ((u32)((arg0->unk0 & 0x3FF) - 0xF0) < 10U)) &&
            (ovl_11_func_800F19E0(arg0->unk22, arg0->unk23) != 0)) {
            ovl_11_func_800F1D54(arg0->unk8, bounds);
            if ((((struct Ovl11ConditionState *)D_8006C838)->date[0] >= bounds[0]) &&
                (bounds[1] >= ((struct Ovl11ConditionState *)D_8006C838)->date[0]) &&
                (ovl_11_func_800F14D8(bounds[2], bounds[3], (s32)((struct Ovl11ConditionState *)D_8006C838)->date) != 0) &&
                (ovl_11_func_800F144C(bounds[4], bounds[5], ((struct Ovl11ConditionState *)D_8006C838)->time) != 0) &&
                (ovl_11_func_800F1540(arg0->unk0, arg0->unk1C) != 0) &&
                (ovl_11_func_800F15B4(arg0->unk2, arg0->unk3, arg0->unk4, arg0->unk6, arg0->unk1C) != 0) &&
                (ovl_11_func_800F1678(arg0->unkC) != 0) &&
                (ovl_11_func_800F1744(arg0->unkD, arg0->unkE, arg0->unk1C) != 0) &&
                (ovl_11_func_800F1878(arg0->unk10, 1) != 0) &&
                (ovl_11_func_800F1878(arg0->unk14, 0) != 0) &&
                (ovl_11_func_800F1954(arg0->unk18) != 0) &&
                (Ovl11TestConditionMask(arg0->unk1C) != 0) &&
                (ovl_11_func_800F1AE0(arg0->unk0) != 0) &&
                (ovl_11_func_800F1AA8(arg0->unk21) != 0)) {
                ovl_11_func_800F12D0(arg0->unk0 & 0x3FF, (u8)(arg0->unk1C >> 8) >> 7);
            }
        }
    }
    return 1;
}
