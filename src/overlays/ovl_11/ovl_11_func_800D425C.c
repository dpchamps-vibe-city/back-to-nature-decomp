#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800D3BB0(u16 *arg0);
void ovl_11_func_800D12B8(Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1, s32 arg2);

s32 ovl_11_func_800D425C(u16 *arg0) {
    if (*arg0 != 0x15B) {
        ovl_11_func_800D12B8((Recon_ovl_11_func_800D2594_A0View *) arg0, ovl_11_func_800D3BB0(arg0), 0x8031);
    } else {
        ovl_11_func_800D12B8((Recon_ovl_11_func_800D2594_A0View *) arg0, ovl_11_func_800D3BB0(arg0), 0x9010);
    }
    if (((*(s32 *) ((u8 *) arg0 + 0x48)) == 0) && ((*(s32 *) ((u8 *) arg0 + 0x4C)) == 0) && ((*(s32 *) ((u8 *) arg0 + 0x50)) == 0)) {
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 0;
    } else {
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 1;
    }
    (*(s32 *) ((u8 *) arg0 + 0x34)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x34)) & ~0x800);
    return 1;
}
