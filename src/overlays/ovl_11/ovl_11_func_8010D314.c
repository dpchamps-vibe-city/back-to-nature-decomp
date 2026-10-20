#include "common.h"
#include "game_types.h"

s32 func_80012A34(s32 arg0);
s32 ovl_11_func_80110658(s16 *arg0, s16 *arg1, s32 arg2, s32 arg3, s32 arg4);
void ovl_11_func_80110E28(u8 *arg0);
u16 ovl_11_func_800D85F8(Recon_ovl_11_func_800D85F8_A0View *arg0);
s32 ovl_11_func_800F3BCC(u16 arg0);
s32 ovl_11_func_800D8320(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
void ovl_11_func_800F397C(u16 arg0, u16 arg1);
void ovl_11_func_800F3AF0(u16 arg0, u16 arg1);
s32 ovl_11_func_8010D648(u8 *arg0, s16 area);
s32 ovl_11_func_8010D748(u8 *arg0, s16 area);
s32 ovl_11_func_8010D850(u8 *arg0, s16 area);

s32 ovl_11_func_8010D314(u8 *arg0) {
    s32 random;
    s32 count;
    s32 i;
    s16 item;
    s16 x;
    s16 y;
    u8 *entry;

    if (*(u16 *)(arg0 + 0xB8) & 1) {
        random = func_80012A34(3);
        count = (u16)(*(u16 *)(arg0 + 0xAE) / 50U) + random;
        for (i = 0; i < count; i++) {
            if (*(u16 *)(arg0 + 0xB6) != 0) {
                item = *(u16 *)(arg0 + 0xB6);
                ovl_11_func_80110E28(arg0);
            } else {
                if (ovl_11_func_80110658(&x, &y, 1, 1, 1)) {
                    if (ovl_11_func_80110658(&x, &y, 0, 1, 1)) {
                        return 0;
                    }
                    entry = (u8 *)&D_80074124[y][x];
                } else {
                    entry = (u8 *)&((Ovl11D124Entry (*)[45])((u8 *)D_80074124 - 0x2328))[y][x];
                }
                item = ovl_11_func_800D85F8((void *)entry);
            }
            ovl_11_func_800F3BCC(item);
        }
    } else if (*(u16 *)(arg0 + 0xB8) & 2) {
        random = func_80012A34(5);
        count = (u16)(*(u16 *)(arg0 + 0xB0) / 10U) + random;
        for (i = 0; i < count; i++) {
            if (ovl_11_func_80110658(&x, &y, 1, 0, 1)) {
                if (ovl_11_func_80110658(&x, &y, 0, 0, 1)) {
                    break;
                }
                ovl_11_func_800D8320(1, x, y, 0);
            } else {
                ovl_11_func_800D8320(0, x, y, 0);
            }
        }
    } else if (*(u16 *)(arg0 + 0xB8) & 4) {
        switch (*(u16 *)(arg0 + 0xB6)) {
        case 0:
            break;
        case 0x68:
            ovl_11_func_800F397C(0, 0);
            break;
        case 0x69:
            ovl_11_func_800F3AF0(0, 0);
            break;
        default:
            ovl_11_func_800F3BCC(*(u16 *)(arg0 + 0xB6));
            break;
        }
        ovl_11_func_80110E28(arg0);
        if (ovl_11_func_8010D648(arg0, 5)) {
            if (!ovl_11_func_8010D748(arg0, 5)) {
                if (!ovl_11_func_8010D850(arg0, 5)) {
                    return 0;
                }
            }
            if (ovl_11_func_8010D648(arg0, 4)) {
                if (ovl_11_func_8010D850(arg0, 4)) {
                    if (!ovl_11_func_8010D748(arg0, 1)) {
                        if (!ovl_11_func_8010D850(arg0, 1)) {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
