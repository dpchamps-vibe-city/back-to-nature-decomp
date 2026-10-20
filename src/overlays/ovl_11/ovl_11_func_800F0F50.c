#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800F1078(Ovl11ConditionRecord *arg0, s32 arg1);

s32 ovl_11_func_800F0F50(void) {
    if (((struct Ovl11C7F0Flags *)D_8006C838)->flags10 & 0x800) {
        return 1;
    }
    if (D_8012961C != 0) {
        ovl_11_func_800F1078((Ovl11ConditionRecord *) ((u8 *) D_8006C838 + 0x2DB8), D_80129618);
    }
    D_8012961C = 0;
    return 1;
}
