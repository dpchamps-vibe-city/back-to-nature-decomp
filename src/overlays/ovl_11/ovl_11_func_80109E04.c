#include "common.h"

#include "ovl11_record_position_set.h"

s32 ovl_11_func_80109E04(u16 *arg0) {
    if (*arg0 == 0) {
        return -1;
    }
    if (gOvl11ActionProbedRecord != 0) {
        Ovl11SetRecordPosition((Ovl11SetFieldsView *)arg0,
            *(Ovl11PaddedVec3 *)&gOvl11ActionProbedRecord[14]);
        return 0;
    }
    return -1;
}
