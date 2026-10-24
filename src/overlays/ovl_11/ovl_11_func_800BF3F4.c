#include "common.h"
#include "game_types.h"

s32 GetVal8005E524(void);

s32 ovl_11_func_800BF3F4(void) {
    u32 *p;
    u32 v;
    u32 r;

    p = GetVal8005E524();
    if (p == 0) {
        return 0;
    }
    v = *p;
    if (v & 4) {
        return 2;
    }
    if (v & 0x40) {
        return 3;
    }
    r = v & 2;
    return r != 0;
}
