#include "common.h"
#include "game_types.h"

void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8001AF70(u16 arg0, u16 arg1);
s32 ovl_11_func_800CE2EC(s32 *arg0);

void ovl_11_func_800CE210(Ovl11C7010Object *arg0) {
    arg0->unk38 = 0;
    arg0->unk6C |= 0x20001800;
    ovl_11_func_800C70CC(arg0, 0x4AU, 0, -2);
    func_8001AF70(9U, 1U);
    ((void (*)(s32 *))ovl_11_func_800CE2EC)(&D_80128CD0.unk0);
    D_80128CE0.unk0 = ((Ovl11CE034Object *)arg0)->unk100;
    D_80128CE0.unk4 = ((Ovl11CE034Object *)arg0)->unk104;
    D_80128CE0.unk8 = ((Ovl11CE034Object *)arg0)->unk108;
    D_80128CD0.unk0 -= 0x226;
    D_80128CD0.unk4 = D_80128CE0.unk4;
    D_80128CD0.unk8 += 0x1B8;
    ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x400000;
    D_80128CEC = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x4000;
}
