#include "common.h"
#include "game_types.h"

s32 func_8001FABC(s16 arg0);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800C4FDC(Ovl11C7010Object *arg0) {
    if ((u32)(*(u16 *)((u8 *)arg0 + 0x8A) - 0x6D) < 2U) {
        func_8001FABC(8);
        arg0->unk6C &= 0xEFFF7FFF;
        ovl_11_func_800C70CC(arg0, 0x34, arg0->unk38 + 0x1D, -2);
    }
}
