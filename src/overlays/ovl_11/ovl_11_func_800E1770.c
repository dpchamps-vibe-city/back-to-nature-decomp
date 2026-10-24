#include "common.h"
#include "game_types.h"

void ovl_11_func_800D0408(u16 arg0, Recon800D0408A1View *arg1, s32 arg2);
void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);

s32 ovl_11_func_800E1770(Ovl11SetFieldsView *arg0) {
    Ovl11PaddedVec3 sp18;
    u8 *base;
    u16 selector;

    base = (u8 *) D_8006C838;
    ovl_11_func_800D0408(*(u16 *) (base + 0x5200), (Recon800D0408A1View *) &sp18, 0x320);
    sp18.field_0 = sp18.field_0 + *(s32 *) ((u8 *) arg0 + 0x38);
    sp18.field_4 = sp18.field_4 + *(s32 *) ((u8 *) arg0 + 0x3C);
    sp18.field_8 = sp18.field_8 + *(s32 *) ((u8 *) arg0 + 0x40);
    ovl_11_func_800D05D0(arg0, sp18);
    selector = *(u16 *) (base + 0x5200);
    *(u16 *) ((u8 *) arg0 + 0x22) = selector;
    return 0;
}
