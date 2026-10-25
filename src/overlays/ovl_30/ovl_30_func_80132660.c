#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"
#include "ovl30_record_menus.h"

s32 ovl_30_func_80132660(s32 arg0) {
    s32 result;

    if (IsScreenFadeComplete() != 0) {
        KanjiFntPrint(D_8012EEB8);
        gOvl30RecordMenuCursor = arg0;
        Ovl30DrawAndEditFlagMenu(&D_801349B8, 0xC);
        result = gOvl30RecordMenuCursor;
    } else {
        result = arg0;
    }
    return result;
}
