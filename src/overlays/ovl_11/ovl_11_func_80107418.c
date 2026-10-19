#include "common.h"
#include "game_types.h"

s32 ovl_11_func_80107480(Ovl11GuardRecord *arg0);

s32 ovl_11_func_80107418(s16 *arg0) {
    Ovl11GuardRecord *record;
    s16 count;
    s32 i;
    s32 result;

    count = arg0[1];
    record = *(Ovl11GuardRecord **)((u8 *)arg0 + 4);
    for (i = 0; i < count; i++, record++) {
        result = ovl_11_func_80107480(record);
        if (result != -1) {
            return result;
        }
    }
    return -1;
}
