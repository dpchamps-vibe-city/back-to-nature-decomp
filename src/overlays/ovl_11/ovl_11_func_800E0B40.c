#include "common.h"
#include "game_types.h"

s16 ovl_11_func_800E0E38(Recon_ovl_11_func_8010CE80_A0View *arg0);
s32 ovl_11_func_800E0FD0(Recon_ovl_11_func_8010CE80_A0View *arg0);

s32 ovl_11_func_800E0B40(Recon_ovl_11_func_8010CE80_A0View *arg0) {
    s32 (*handler)(void *);
    char *far_base;

    if (*(u16 *)arg0 == 0) {
        return -1;
    }
    far_base = (char *)&D_8007AFF0;
    if (((*(s16 *)(far_base + 0x25476) == 1) ||
         (*(s16 *)(far_base + 0x25476) == 5)) &&
        (*(s32 *)((u8 *)arg0 + 0x8C) == 0)) {
        ovl_11_func_800E0E38(arg0);
    }
    handler = D_800B966C[(u16)arg0->unk26];
    if (handler != 0) {
        handler(arg0);
    }
    if (!(arg0->unk34 & 0x800) && arg0->unk2C >= 0x12C) {
        ovl_11_func_800E0FD0(arg0);
        arg0->unk2C = 0;
    }
    arg0->unk2C++;
    return 0;
}
