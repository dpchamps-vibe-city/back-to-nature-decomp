#include "common.h"

extern u16 D_80070D06;
extern u16 gTaggedRecordPoolExpanded;
extern u16 gB4RecordPoolExpanded;
extern u16 D_80070D0C;

s32 ovl_11_func_800EC490(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    if (arg0 != -1) {
        gOvl11ValueSlots[arg0] = D_80070D06;
    }
    if (arg1 != -1) {
        gOvl11ValueSlots[arg1] = gTaggedRecordPoolExpanded;
    }
    if (arg2 != -1) {
        gOvl11ValueSlots[arg2] = gB4RecordPoolExpanded;
    }
    if (arg3 != -1) {
        gOvl11ValueSlots[arg3] = D_80070D0C;
    }
    return 1;
}
