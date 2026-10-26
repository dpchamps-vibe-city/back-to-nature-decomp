#include "common.h"
#include "ovl11_record_family_lookup.h"

s32 ovl_11_func_80118C7C(s16 arg0, s16 arg1, s16 arg2) {
    s32 callRet1;
    callRet1 = Ovl11FindFamilyRecordIndex(arg0, arg1, arg2);
    return callRet1;
}
