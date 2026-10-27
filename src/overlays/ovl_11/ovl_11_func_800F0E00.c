#include "common.h"
#include "psyq/memory.h"



void ovl_11_func_800F0E00(void *arg0) {
    u16 *list;
    u16 term;
    u16 loopTerm;
    s32 count;

    gOvl11ConditionRecordCount = 0;
    if (arg0 == 0) {
        gOvl11ConditionRecordCount = 0;
    } else {
        list = &D_8006F5F0;
        memset(list, 0, 0xE10);
        memcpy(list, arg0, 0xE10);
        term = 0xFFFF;
        count = 0;
        if (*list == term) {
            gOvl11ConditionRecordCount = 0;
        } else {
            do {
                loopTerm = 0;
                count += 1;
                loopTerm = 0xFFFF;
            } while (*((u16 *)((u8 *)list + count * 0x24)) != loopTerm);
            gOvl11ConditionRecordCount = count;
        }
    }
    gOvl11ConditionEvaluationState = 0;
}
