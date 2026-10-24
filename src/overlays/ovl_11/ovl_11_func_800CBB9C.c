#include "common.h"
#include "game_types.h"

void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800D666C(s16 arg0);
void ovl_11_func_800D7328(s16 *arg0);
void ovl_11_func_800D7348(u16 *dst, u16 *src);
s32 ovl_11_func_800D6A94(s16 arg0);
s32 func_8001FABC(s16 arg0);

void ovl_11_func_800CBB9C(Ovl11C7010Object *arg0, s32 arg1, s32 arg2) {
    u16 sp10[3];
    s16 *temp_s1;
    s16 *temp_s3;
    s16 temp_a0;
    s32 temp_a1;
    s32 var_s2;
    u16 temp_a0_2;

    var_s2 = arg2;
    sp10[0] = arg1;
    sp10[1] = 0;
    sp10[2] = 1;
    ovl_11_func_800C70CC(arg0, 3U, 5, -2);
    D_80128C58 = *(Ovl11ItemState *)((u8 *)arg0 + 0x8A);
    temp_a0 = arg0->itemIndex;
    temp_s3 = &arg0->itemIndex;
    if (temp_a0 != 0) {
        ovl_11_func_800D666C(temp_a0);
    }
    ovl_11_func_800D7328(temp_s3);
    temp_s1 = (s16 *)((u8 *)arg0 + 0xF0);
    ovl_11_func_800D7328(temp_s1);
    ovl_11_func_800D7348((u16 *)temp_s1, sp10);
    ovl_11_func_800D6A94(*(s16 *)((u8 *)arg0 + 0xF0));
    temp_a1 = arg0->unk6C & ~0x4000;
    arg0->unk6C = temp_a1;
    temp_a0_2 = ((Ovl11CBB9CGlobals *)D_8006C838)->items[*(s16 *)((u8 *)arg0 + 0xF0)].u0.flags;
    if ((temp_a0_2 & 0x1200) == 0x1200) {
        var_s2 = 2;
    }
    if ((temp_a0_2 & 0x1600) == 0x1600) {
        var_s2 = 3;
    }
    switch (var_s2) {
    case 0:
    case 1:
        ovl_11_func_800D7348((u16 *)temp_s3, (u16 *)temp_s1);
        ovl_11_func_800C70CC((Ovl11C7010Object *)((Ovl11CBB9CGlobals *)D_8006C838)->object, 0x30U, ((Ovl11CBB9CGlobals *)D_8006C838)->state + 0x21, -2);
        return;
    case 2:
        arg0->unk6C = temp_a1 | 0x8000;
        ovl_11_func_800D7348((u16 *)temp_s3, (u16 *)temp_s1);
        ovl_11_func_800C70CC(arg0, 0x31U, 5, -2);
        func_8001FABC(0x4A);
        return;
    case 3:
        ovl_11_func_800D7348((u16 *)temp_s3, (u16 *)temp_s1);
        arg0->unk6C |= 0x5800;
        ovl_11_func_800C70CC(arg0, 0x4CU, 0, -2);
        func_8001FABC(0x4B);
        return;
    }
}
