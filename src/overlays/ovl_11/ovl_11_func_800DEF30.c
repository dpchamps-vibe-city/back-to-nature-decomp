#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800DF4F0(UnkStruct800DF4F0 *arg0);
s16 ovl_11_func_800DF228(Recon_ovl_11_func_800DEEE0_A0View *arg0);
s32 ovl_11_func_800DF400(UnkStruct800DF4F0 *arg0);

s32 ovl_11_func_800DEF30(UnkStruct800DF4F0 *arg0) {
    s32 (*handler)(void *);
    char *far_base;

    if (ovl_11_func_800DF4F0(arg0) == 0) {
        return -1;
    }
    far_base = (char *)&D_8007AFF0;
    if (((*(s16 *)(far_base + 0x25476) == 1) ||
         (*(s16 *)(far_base + 0x25476) == 5)) &&
        (*(s32 *)((u8 *)arg0 + 0x8C) == 0)) {
        ovl_11_func_800DF228((Recon_ovl_11_func_800DEEE0_A0View *)arg0);
    }
    handler = D_800B95CC[*(u16 *)((u8 *)arg0 + 0x26)];
    if (handler != 0) {
        handler(arg0);
    }
    if (!(*(s32 *)((u8 *)arg0 + 0x34) & 0x800) &&
        (*(s16 *)((u8 *)arg0 + 0x2C) >= 0x12C)) {
        ovl_11_func_800DF400(arg0);
        *(s16 *)((u8 *)arg0 + 0x2C) = 0;
    }
    (*(s16 *)((u8 *)arg0 + 0x2C))++;
    return 0;
}
