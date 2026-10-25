#include "common.h"
#include "random.h"

extern s16 gOvl19ActorPairState[];

s32 ovl_19_func_800BA2D4(void) {
    s32 var_s0;

    if (gOvl19ActorPairState[4] != gOvl19ActorPairState[0x28]) {
        var_s0 = gOvl19ActorPairState[4] <= gOvl19ActorPairState[0x28];
        if (Rand(100) >= 0x5B) {
            var_s0 = !var_s0;
        }
    } else {
        var_s0 = Rand(2) != 0;
    }
    return var_s0;
}
