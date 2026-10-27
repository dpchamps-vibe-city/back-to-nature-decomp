#include "common.h"

extern s32 *gOvl11ActionProbedRecord;

/* Callee prototype as the original caller TU saw it: the trailing words are
 * plain words in the outgoing area (functions.h holds the callee-side
 * aggregate type). */
void ovl_11_func_800D05D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

s32 ovl_11_func_80109E04(u16 *arg0) {
    if (*arg0 == 0) {
        return -1;
    }
    if (gOvl11ActionProbedRecord != 0) {
        ovl_11_func_800D05D0((s32)arg0, gOvl11ActionProbedRecord[14], gOvl11ActionProbedRecord[15], gOvl11ActionProbedRecord[16], gOvl11ActionProbedRecord[17]);
        return 0;
    }
    return -1;
}
