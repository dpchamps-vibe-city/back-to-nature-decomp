#include "common.h"
#include "game_types.h"

void ovl_11_func_800DB354(Ovl11AnimationSlot *arg0);
void ovl_11_func_800DB4E0(Ovl11AnimationSlot *arg0);

void ovl_11_func_800DB5CC(void) {
    Ovl11AnimationSlot *var_s0;
    s32 var_s1;
    u16 temp_v1;

    var_s0 = (Ovl11AnimationSlot *) &D_800A03B0;
    for (var_s1 = 0; var_s1 < 8; var_s1++, var_s0++) {
        temp_v1 = var_s0->unk0;
        if ((temp_v1 != 0xFFFF) && !(((struct struct_8006C838_view *) D_8006C838)->field_0C & 0x08000000)) {
            if (temp_v1 & 0x8000) {
                ovl_11_func_800DB354(var_s0);
            } else {
                ovl_11_func_800DB4E0(var_s0);
            }
        }
    }
}
