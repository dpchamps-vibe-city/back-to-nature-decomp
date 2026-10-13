#include "common.h"
#include "game_types.h"

void ovl_11_func_80121500();

s32 ovl_11_func_8012135C(s32 arg0, Ovl11EffectCoordinate arg1, Ovl11EffectCoordinate arg2, Ovl11EffectCoordinate arg3, Ovl11EffectCoordinate arg4) {
    Ovl11SpriteEffect *var_v1;
    s32 var_t0;

    var_v1 = (Ovl11SpriteEffect *)Ovl11_D_8012DB90;
    var_t0 = 0;
    arg0 &= 0xFFFF;
    for (; var_t0 < 0x19; var_t0++) {
        if (var_v1->unk14 == 0) {
            ovl_11_func_80121500(var_v1, *(s32 *)&arg1, *(s32 *)&arg2, *(s32 *)&arg3, *(s32 *)&arg4, arg0);
            return 1;
        }
        var_v1++;
    }
    return 0;
}
