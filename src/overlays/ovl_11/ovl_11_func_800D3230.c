#include "common.h"
#include "psyq/memory.h"
#include "ovl11_record_selection.h"

s32 ovl_11_func_800D3230(u16 arg0) {
    s32 callRet1;
    callRet1 = Ovl11ChooseEligibleTaggedRecord(arg0);
    return callRet1;
}
