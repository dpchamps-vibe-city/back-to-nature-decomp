#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800D58D0(void *arg0);
void ovl_11_func_800CCF14(Ovl11ItemState *arg0);
s32 ovl_11_func_800D6A94(s16 arg0);

void ovl_11_func_800C6770(Ovl11C6770Object *arg0) {
    Ovl11ItemState *temp_s1;

    temp_s1 = &arg0->item;
    ovl_11_func_800D58D0(temp_s1);
    if (arg0->item.id == 0x126) {
        arg0->unk6C |= 0x200;
    }
    ovl_11_func_800CCF14(temp_s1);
    if (arg0->unk3C == 0x31) {
        arg0->item = D_80128C58;
        ovl_11_func_800D6A94(arg0->item.id);
        arg0->unk6C &= ~0x4000;
        arg0->unk6C |= 0x8000;
        if (arg0->item.id == 0) {
            arg0->unk6C &= ~0x10000000;
            arg0->unk6C &= ~0x8000;
        }
    } else {
        arg0->unk6C &= ~0x10000000;
        arg0->unk6C &= ~0x8000;
    }
}
