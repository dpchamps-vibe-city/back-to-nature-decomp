#include "common.h"
#include "psyq/memory.h"

void ovl_11_func_800C0FC0(s16 arg0);
void ovl_11_func_800C11C4(s32 arg0);
s32 ovl_11_func_800C1224(s16 arg0, s16 arg1);
void func_8001AF70(u16 arg0, u16 arg1);

void ovl_11_func_800C090C(void) {
    struct Ovl11StateInitView *state = (struct Ovl11StateInitView *) D_8006C838;

    state->field_44BC = 1;
    state->field_44BE = 1;
    state->field_44B8 = 0;
    state->field_44CA = 0x168;
    state->field_44BA = 0;
    state->field_44C2 = 0;
    state->field_44C8 = 0;
    state->field_D4 = 0;
    state->field_44C0 = 6;
    memset(&D_80128A88, -1, 0xC8);
    state->field_D0 = 0;
    D_80128B54 = 0;
    D_80128B50 = 0;
    D_80128B58 = 0;
    ovl_11_func_800C0FC0(state->field_44BE);
    ovl_11_func_800C11C4(state->field_44BA);
    if (ovl_11_func_800C1224(state->field_44BA, state->field_44BC) != 0) {
        func_8001AF70(0x2B, 1);
    }
    func_8001AF70(0x96, 1);
}
