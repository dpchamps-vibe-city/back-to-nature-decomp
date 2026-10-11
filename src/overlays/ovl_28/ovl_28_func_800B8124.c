#include "common.h"
#include "game_types.h"

void func_80015EE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80015114(u32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s16 arg6);
void func_800132B8(s32 arg0, s32 arg1, s32 arg2);
s32 func_80013394(void);
void func_800132F0(s32 arg0, s32 arg1, s32 arg2);

void ovl_28_func_800B8124(void) {
    s32 *base;

    func_80015EE8(D_8005E3C0->field_D8 + 0x18, (s32) D_800B9DC0, 0x1F, 0, 0, -7);
    func_80015114((u32 *)(D_8005E3C0->field_D8 + 0x14), 0, 0, *(s16 *)&HWD0, 0x8C, 0, 1);
    D_800B9620 = (D_800B9620 + 1) % 108000;
    switch (D_800B9624) {                           /* irregular */
    case 0x0:
        func_800132B8(0xA, 0, 2);
        D_800B9624 = (u16) D_800B9624 + 1;
        return;
    case 0x1:
        if (func_80013394() == 1) {
            D_800B9624 = (u16) D_800B9624 + 1;
        }
        return;
    case 0x2:
        D_800B9614();
        D_800B9618();
        if (D_800B93BC == 1) {
            func_800132F0(0xA, 0, 2);
            D_800B9624 = (u16) D_800B9624 + 1;
        }
        break;
    case 0x3:
        if (func_80013394() == 1) {
            D_800B9624 = 0xFF;
            return;
        }
        break;
    case 0xFF:
        base = (s32 *)&D_8006C838;
        base[0x1122] = base[0x1122] + 1;
        break;
    }
}
