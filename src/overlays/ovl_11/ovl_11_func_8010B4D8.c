#include "common.h"

s32 ovl_11_func_8010B8DC(u16 *arg0);

s32 ovl_11_func_8010B4D8(u16 *arg0) {
    s32 (*handler)(u16 *);

    if (*arg0 == 0) {
        return -1;
    }
    handler = (s32 (*)(u16 *)) D_800BAA34[*(u16 *) ((u8 *) arg0 + 0x26)];
    if (handler != 0) {
        handler(arg0);
    }
    if (!(*(s32 *) ((u8 *) arg0 + 0x34) & 0x800) &&
        *(s16 *) ((u8 *) arg0 + 0x2C) >= 300) {
        ovl_11_func_8010B8DC(arg0);
        *(s16 *) ((u8 *) arg0 + 0x2C) = 0;
    }
    (*(s16 *) ((u8 *) arg0 + 0x2C))++;
    return 0;
}
