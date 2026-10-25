#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"

void ovl_30_func_80133454(Ovl30ValueMenuEntry *arg0, s32 arg1);

s32 ovl_30_func_80130690(s32 arg0) {
    s32 result;

    if (IsScreenFadeComplete() != 0) {
        KanjiFntPrint(D_8012E848);
        D_80134014 = arg0;
        ovl_30_func_80133454(&D_801345E0, 0x30);
        result = D_80134014;
    } else {
        result = arg0;
    }
    return result;
}
