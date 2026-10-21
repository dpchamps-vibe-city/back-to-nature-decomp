#include "common.h"

s32 ovl_11_func_800D0BC8(s32 arg0);
s32 *ovl_11_func_800D0CD8(void);
s32 ovl_11_func_800E2934(void *arg0);

s32 ovl_11_func_800D0F20(s32 arg0, s32 arg1) {
    s32 slot;
    struct Ovl11ResetB4 *record;

    slot = arg0 + 2;
    if (ovl_11_func_800D0BC8(slot) != 0) {
        return 0x1E6;
    }
    record = (struct Ovl11ResetB4 *)ovl_11_func_800D0CD8();
    if (record != 0) {
        record->flags |= 0x02000000;
        ((struct Ovl11PoolResetView *)D_8006C838)->pairs_99C8[slot][0] = ovl_11_func_800E2934(record);
        ((struct Ovl11PoolResetView *)D_8006C838)->pairs_99C8[slot][1] = arg1;
        return 0;
    }
    return 0x1A1;
}
