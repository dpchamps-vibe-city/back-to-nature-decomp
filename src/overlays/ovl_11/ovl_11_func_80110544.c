#include "common.h"
#include "game_types.h"

void ovl_11_func_800D1960(s16 arg0);
s32 ovl_11_func_8010E644(Ovl11E644Object *arg0);
s32 ovl_11_func_800D0408(s16 arg0, Recon800D0408A1View *arg1, s32 arg2);

s32 ovl_11_func_80110544(Ovl11E644Object *arg0, s16 arg1) {
    s32 temp_s0;

    temp_s0 = arg0->unk34;
    ovl_11_func_800D1960(arg1);
    ovl_11_func_8010E644(arg0);
    if (temp_s0 & 0x800) {
        arg0->unk34 |= 0x800;
    }
    if (!(arg0->unk34 & 0x2000)) {
        ovl_11_func_800D0408(4, (Recon800D0408A1View *) &arg0->unk48, 0);
        return 1;
    }
    return 0;
}
