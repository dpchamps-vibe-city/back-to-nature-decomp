#include "common.h"

extern s32 gOvl11InteractionStep;
extern s32 gOvl11InteractionResumeStep;

void ovl_11_func_8011D0B4(void) {
    gOvl11InteractionStep = gOvl11InteractionResumeStep;
}
