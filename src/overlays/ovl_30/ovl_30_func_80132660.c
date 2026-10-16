#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 func_80013394(void);
void ovl_30_func_80133A8C(Ovl30FlagMenuEntry *arg0, s32 arg1);

s32 ovl_30_func_80132660(s32 arg0) {
    s32 result;

    if (func_80013394() != 0) {
        KanjiFntPrint(D_8012EEB8);
        D_80134014 = arg0;
        ovl_30_func_80133A8C(&D_801349B8, 0xC);
        result = D_80134014;
    } else {
        result = arg0;
    }
    return result;
}
