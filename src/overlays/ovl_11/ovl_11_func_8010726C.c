#include "common.h"
#include "game_types.h"

void ovl_11_func_80107044(Ovl11RotationState *arg0);

void ovl_11_func_8010726C(void) {
    Ovl11RotationState *var_s0;
    s32 state;
    u32 var_s1;

    var_s0 = Ovl11_D_8012CF48;
    if (!(D_8006C844 & 0x08000000)) {
        for (var_s1 = 0; var_s1 < 10U; var_s1++, var_s0++) {
            if (var_s0->unk0 != 0) {
                state = var_s0->unk0;
                switch (state) {
                case 2:
                    ovl_11_func_80107044(var_s0);
                    break;
                case 3:
                    var_s0->unk4--;
                    if (var_s0->unk4 == 0) {
                        var_s0->unk0 = 4;
                    }
                    break;
                case 4:
                    ovl_11_func_80107044(var_s0);
                    break;
                }
            }
        }
    }
}
