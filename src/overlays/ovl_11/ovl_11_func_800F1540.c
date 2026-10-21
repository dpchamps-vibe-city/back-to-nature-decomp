#include "common.h"

s32 ovl_11_func_800F1540(u16 arg0, s32 arg1) {
    s32 kind;

    kind = arg0 >> 10;
    if (kind == 0x3F) {
        return 1;
    }
    return (arg1 < 0) ? (kind != ((Ovl11WorkHeaderView *)&D_8007AFF0)->header->unk2)
                      : (kind == ((Ovl11WorkHeaderView *)&D_8007AFF0)->header->unk2);
}
