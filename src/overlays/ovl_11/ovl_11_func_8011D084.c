#include "common.h"

extern s32 gOvl11InteractionStep;
extern s32 gOvl11InteractionResumeStep;

void ovl_11_func_8011D084(s32 arg0, s32 arg1) {
    gOvl11InteractionStep = arg0;
    gOvl11InteractionResumeStep = arg1;
}
