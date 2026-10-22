#include "common.h"

s32 ovl_11_func_800F1EE4(s32 arg0);

s32 ovl_11_func_800F1678(u8 arg0) {
    s32 id;
    s32 flags;
    s16 index;
    char *record;
    char *base;

    flags = arg0;
    if (((Ovl11EF488SelectionView *)&D_8006C838)->selected == -1) {
        if (!(flags & 0x40)) {
            goto zero;
        }
    } else if (!(flags & 0x80)) {
        goto zero;
    }
    id = flags & 7;
    if (id == 7) {
        return 1;
    }
    index = ovl_11_func_800F1EE4(id);
    base = (char *)&D_8006C838;
    record = base + index * 0x1D4;
    if (!(*(u16 *)(record + 0x9A06) & 8)) {
        goto other;
    }
    if (flags & 0x20) {
        goto one;
    }
zero:
    return 0;
other:
    if (!(flags & 0x10)) {
        return 0;
    }
one:
    return 1;
}
