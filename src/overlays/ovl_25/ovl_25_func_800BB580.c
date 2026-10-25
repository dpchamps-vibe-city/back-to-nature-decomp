#include "common.h"
#include "game_types.h"

void ovl_25_func_800BB628(Ovl11AnimationSlot *arg0);
void ovl_25_func_800BB788(Ovl11AnimationSlot *arg0);

void ovl_25_func_800BB580(void) {
    s32 var_s1;
    Ovl11AnimationSlot *var_s0;
    u16 temp_v1;

    var_s0 = (Ovl11AnimationSlot *) &gVramAnimationSlotHead;
    for (var_s1 = 0; var_s1 < 8; var_s1++, var_s0++) {
        temp_v1 = var_s0->key;
        if ((temp_v1 != 0xFFFF) && !(((struct struct_8006C838_view *) D_8006C838)->field_0C & 0x08000000)) {
            if (temp_v1 & 0x8000) {
                ovl_25_func_800BB628(var_s0);
            } else {
                ovl_25_func_800BB788(var_s0);
            }
        }
    }
}
