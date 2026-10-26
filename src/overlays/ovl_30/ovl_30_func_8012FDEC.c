#include "common.h"
#include "game_types.h"

extern s32 gOvl30DebugSettingValue;

void ovl_30_func_8012FDEC(void) {
    if (((SomeStruct *)gPad0Input)->field_0x0 & 0x2000) {
        gOvl30DebugSettingValue += 1;
        if (gOvl30DebugSettingValue == 0x100) {
            gOvl30DebugSettingValue = 0;
        }
    }

    if (((SomeStruct *)gPad0Input)->field_0x0 & 0x8000) {
        if (gOvl30DebugSettingValue == 0) {
            gOvl30DebugSettingValue = 0x100;
            return;
        }
        gOvl30DebugSettingValue -= 1;
    }
}
