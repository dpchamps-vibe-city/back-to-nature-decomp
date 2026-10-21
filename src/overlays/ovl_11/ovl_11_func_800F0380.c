#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800F0380(s16 arg0) {
    if ((arg0 == 1 && ((Ovl11Status99C8View *)D_8006C838)->field_44D0 == 0) ||
        (arg0 == 3 && ((Ovl11Status99C8View *)D_8006C838)->field_44D2 == 0)) {
        return 0;
    }
    if (!(arg0 & 2)) {
        if (((struct Ovl11PoolResetView *)D_8006C838)->records_81BC[
                ((struct Ovl11PoolResetView *)D_8006C838)->pairs_99C8[arg0][1]].flags & 0x200) {
            return 1;
        }
    } else {
        if (((struct Ovl11PoolResetView *)D_8006C838)->records_7AB4[
                ((struct Ovl11PoolResetView *)D_8006C838)->pairs_99C8[arg0][0]].type == 0x10A) {
            return 1;
        }
    }
    return 0;
}
