#include "common.h"
#include "game_types.h"

void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, u16 arg1, s32 arg2, s32 arg3);
s32 func_8001FABC(s16 arg0);
s32 ovl_11_func_800D5750(s16 arg0);
void ovl_11_func_800D7348(u16 *dst, u16 *src);
void ovl_11_func_800D7328(s16 *arg0);
s32 ovl_11_func_800C69B8(Ovl11C7010Object *arg0);
s32 ovl_11_func_800CB4E0(void);

s32 ovl_11_func_800C6858(Ovl11C7010Object *arg0) {
    u16 *item;
    s32 index;

    if (arg0->unk3C == 0xE) {
        arg0->unk6C |= 0x08000000;
    } else {
        arg0->unk6C |= 0x10000000;
    }
    arg0->unk6C &= 0xFFDFFFFF;
    if (arg0->unk3C == 0x30) {
        ovl_11_func_800C70CC(arg0, 0x31, 5, -2);
        func_8001FABC(0x4A);
        arg0->unk6C |= 0x8000;
        return 1;
    }
    index = ovl_11_func_800D5750(*(s16 *) ((u8 *) arg0 + 0xF0));
    if (index == 2) {
        index = 1;
    }
    item = (u16 *) ((u8 *) arg0 + 0xF0);
    ovl_11_func_800D7348((u16 *) ((u8 *) arg0 + (index * 6 + 0x84)), item);
    ovl_11_func_800D7328((s16 *) item);
    arg0->unk6C |= 0x8000;
    if (arg0->itemIndex == 0x66) {
        func_8001FABC(8);
        arg0->unk6C &= 0xEFFF7FFF;
        ovl_11_func_800C70CC(arg0, 0x33, arg0->unk38 + 0x1D, -2);
        return 1;
    }
    if (ovl_11_func_800C69B8(arg0) == 0) {
        ovl_11_func_800CB4E0();
        return 0;
    }
    return 1;
}
