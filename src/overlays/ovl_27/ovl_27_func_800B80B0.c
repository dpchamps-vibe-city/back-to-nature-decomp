#include "common.h"
#include "psyq/memory.h"

u32 func_80012A34(s32 arg0);
s32 ovl_27_func_800BA230(void);
void ovl_27_func_800B9324(void);
void func_800132B8(s32 arg0, s32 arg1, s32 arg2);

void ovl_27_func_800B80B0(void) {
    env_struct_0x134 *second;
    Ovl27StateCounterView *state;
    Ovl27StateCounterView *state2;

    if (ovl_27_func_800BA230() != 0) {
        memcpy(D_800605E8, D_800C4A40[func_80012A34(3)], 0x1800);
        if (D_800C4A28 & 0x4000) {
            D_800C4A18 = 7;
            state = (Ovl27StateCounterView *)D_8006C838;
            state->counter++;
        } else {
            D_8005E5E8->unk19 = 0xFF;
            D_8005E5E8->unk1A = 0xFF;
            D_8005E5E8->unk1B = 0xFF;
            second = &D_8005E5E8[1];
            second->unk19 = 0xFF;
            second->unk1A = 0xFF;
            second->unk1B = 0xFF;
            D_8005E5E8->unk18 = 1;
            D_8005E5E8[1].unk18 = 1;
            ovl_27_func_800B9324();
            func_800132B8(0x1E, 0, 2);
            D_800C4A18 = 0;
            state2 = (Ovl27StateCounterView *)D_8006C838;
            state2->counter++;
        }
    }
}
