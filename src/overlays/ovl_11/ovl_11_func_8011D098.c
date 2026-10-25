#include "common.h"

extern s32 gOvl11InteractionStep;
extern s32 gOvl11InteractionResumeStep;

void ovl_11_func_8011D098(s32 arg0, s32 arg1, s32 arg2) {
    gOvl11InteractionStep = arg0 + arg2;
    gOvl11InteractionResumeStep = arg1 + arg2;
}
