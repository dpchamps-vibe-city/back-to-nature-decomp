#include "common.h"

s32 ovl_11_func_80107B84(s32 arg0);
s32 ovl_11_func_80107418(s16 *arg0);

s32 ovl_11_func_80107390(s32 arg0) {
    s16 *record;
    u32 i;

    D_8012D038 = -1U;
    for (i = 0; i < 31; i++) {
        record = D_80127814[i];
        if (record[0] == arg0) {
            if (ovl_11_func_80107B84(arg0) == 1) {
                return 0x1CD;
            }
            return ovl_11_func_80107418(record);
        }
    }
    return -1;
}
