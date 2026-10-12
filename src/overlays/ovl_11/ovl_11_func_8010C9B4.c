#include "common.h"
#include "game_types.h"

s32 ovl_11_func_8010CB6C(Ovl11ObjectCB6C *arg0);

void ovl_11_func_8010C9B4(void) {
    s32 var_s1;
    u8 *var_s0;

    var_s0 = D_8007A638;
    var_s1 = 0;
    do {
        if (!(((Ovl11ObjectCB6C *)var_s0)->unkB8 & 0x20)) {
            ovl_11_func_8010CB6C((Ovl11ObjectCB6C *) var_s0);
        }
        var_s1 += 1;
        var_s0 += 0xF8;
    } while (var_s1 < 7);
}
