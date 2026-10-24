#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void ovl_25_func_800B9B28 (Ovl25ObjectRecord *arg0);
void ovl_25_func_800B9C90 (Ovl25ObjectRecord *arg0);
void func_80015840(ObjectState *obj, u8 arg1);
void ovl_25_func_800BA5BC (SpriteSourceData *arg0, u16 *arg1);
void ovl_25_func_800BA758 (SpriteSourceData *arg0, u16 *arg1);

void ovl_25_func_800B941C(void) {
    Ovl25ObjectRecord *var_s1;
    Ovl25ObjectRecord *var_s1_2;
    s32 var_s0;
    s32 var_s0_2;

    var_s1 = &D_800BFE54;
    var_s0 = 5;
    do {
        ovl_25_func_800B9B28(var_s1);
        var_s0 -= 1;
        var_s1 += 1;
    } while (var_s0 >= 0);
    var_s1_2 = &D_800C0124;
    var_s0_2 = 1;
    do {
        ovl_25_func_800B9C90(var_s1_2);
        var_s0_2 -= 1;
        var_s1_2 += 1;
    } while (var_s0_2 >= 0);
}
