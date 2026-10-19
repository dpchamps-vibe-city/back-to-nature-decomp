#include "common.h"
#include "game_types.h"

void ovl_11_func_800CC0F8(Ovl11CC7CCObject *arg0);

void ovl_11_func_800CCB90(void) {
    Ovl11CC7CCObject *var_s0;
    u32 var_s1;

    var_s0 = (Ovl11CC7CCObject *) D_80128BB0;
    var_s1 = 0;
    do {
        if (var_s0->unk2 != 0) {
            ovl_11_func_800CC0F8(var_s0);
        }
        var_s1 += 1;
        var_s0 += 1;
    } while (var_s1 < 3U);
}
