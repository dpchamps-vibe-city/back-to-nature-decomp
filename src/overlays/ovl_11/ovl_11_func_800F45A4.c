#include "common.h"
#include "game_types.h"
#include "ovl11_record_pool_refresh.h"

void ovl_11_func_800F45A4(s16 arg0, s32 arg1) {
    Ovl11DispatchRecord *v1;
    s32 *base;
    s32 *base2;

    base = (s32 *)&D_8006C838;
    base2 = base + (0x8000 >> 2);
    v1 = Ovl11FindDispatchRecordByKind(arg0, (Ovl11DispatchRecord *)base2[0x5DB4 >> 2], 13);
    if (v1 != 0) {
        if (arg1 == 1) {
            v1->flags |= 1;
        } else {
            v1->flags &= 0xFFFE;
        }
    }
}
