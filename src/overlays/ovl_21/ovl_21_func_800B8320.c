#include "common.h"

void ovl_21_func_800B836C(void);

void ovl_21_func_800B8320(void) {
    u8 *state;

    state = (u8 *) D_800C0448;
    *(s32 *) (state + 4) = 0;
    *(s16 *) (state + 0x984) = -1;
    *(s16 *) (state + 0x986) = -1;
    *(s16 *) (state + 0x988) = 0;
    *(s16 *) (state + 8) = *(s16 *) (state + 0x64C) * 30;
    ovl_21_func_800B836C();
}
