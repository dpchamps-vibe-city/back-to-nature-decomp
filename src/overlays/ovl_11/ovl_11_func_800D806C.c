#include "common.h"

s32 ovl_11_func_800D806C(s16 arg0, s16 arg1, s16 arg2, s16 arg3) {
    struct_80123E04 *bounds;
    Ovl11GridTextureEntry *entry;

    bounds = &D_80123E04 + arg3;
    if ((arg0 < 0) || (arg0 >= bounds->unk0)) {
        return 1;
    }
    if ((arg1 < 0) || (arg1 >= bounds->unk2)) {
        return 1;
    }
    entry = ((Ovl11GridTextureRowsView *)&D_8007AFF0)->entries[arg1][arg0];
    if (arg2 != 0) {
        if (!(entry->header & 8)) {
            return 1;
        }
    }
    return 0;
}
