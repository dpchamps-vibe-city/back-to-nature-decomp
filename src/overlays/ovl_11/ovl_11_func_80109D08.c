#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "ovl11_record_position_set.h"

s32 ovl_11_func_801096FC(Ovl11PaddedVec3 position, s16 key, s32 radius, s32 *flag, s32 **record);

s32 ovl_11_func_80109D08(Ovl11SetFieldsView *arg0) {
    s16 temp_v1;
    s32 var_t0;
    s32 found;

    if (*(u16 *) arg0 == 0) {
        return -1;
    }
    if (!(arg0->field_34 & 0x8000)) {
        return -1;
    }
    temp_v1 = *(s16 *) ((u8 *) arg0 + 0xAC);
    var_t0 = 0x4B0;
    if (temp_v1 >= 0x32) {
        var_t0 = 0x640;
        if (temp_v1 >= 0x64) {
            var_t0 = 0x7D0;
            if (temp_v1 >= 0x96) {
                var_t0 = 0xFA0;
                if (temp_v1 < 0xC8) {
                    var_t0 = 0xAF0;
                }
            }
        }
    }
    gOvl11ActionFoundRecord = NULL;
    found = ovl_11_func_801096FC(
        *(Ovl11PaddedVec3 *) ((u8 *) arg0 + 0x38), arg0->field_30,
        var_t0, &gOvl11ActionFoundRecordCategory, &gOvl11ActionFoundRecord);
    if (found != 1) {
        found = -1;
    } else {
        Ovl11SetRecordPosition(arg0,
            *(Ovl11PaddedVec3 *) ((u8 *) gOvl11ActionFoundRecord + 0x38));
        found = 0;
    }
    return found;
}
