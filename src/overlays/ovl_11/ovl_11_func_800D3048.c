#include "common.h"

void ovl_11_func_800D3574(u16 *record);
s32 ovl_11_func_800D3468(u16 *arg0);

s32 ovl_11_func_800D3048(u16 *arg0) {
    s32 (*handler)(u16 *);

    if (*arg0 == 0) {
        return -1;
    }
    ovl_11_func_800D3574(arg0);
    if (*arg0 == 0) {
        return -1;
    }
    handler = (s32 (*)(u16 *)) D_800B8EC0[*(u16 *) ((u8 *) arg0 + 0x26)];
    if (handler != 0) {
        handler(arg0);
    }
    if (!(*(s32 *) ((u8 *) arg0 + 0x34) & 0x800) &&
        *(s16 *) ((u8 *) arg0 + 0x2C) >= 300) {
        ovl_11_func_800D3468(arg0);
        *(s16 *) ((u8 *) arg0 + 0x2C) = 0;
    }
    (*(s16 *) ((u8 *) arg0 + 0x2C))++;
    return 0;
}
