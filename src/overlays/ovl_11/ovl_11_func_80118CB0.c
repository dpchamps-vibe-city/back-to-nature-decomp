#include "common.h"

extern s32 D_80128210;
extern s32 gOvl11InteractionStep;
extern s32 gOvl11InteractionDrawMode;
extern s16 gOvl11QuantityPromptCount;

s32 ovl_11_func_80118CB0(void) {
    gOvl11InteractionId = 0;
    D_80128210 = 1;
    gOvl11InteractionStep = 0;
    gOvl11InteractionDrawMode = 0;
    gOvl11QuantityPromptCount = 1;
    return 1;
}
