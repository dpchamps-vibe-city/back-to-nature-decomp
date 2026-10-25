#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"
#include "ovl30_record_menus.h"

s32 ovl_30_func_8012FF50(s32 arg0) {
    s32 result;

    if (IsScreenFadeComplete() != 0) {
        KanjiFntPrint(D_8012E29C);
        gOvl30RecordMenuCursor = arg0;
        Ovl30DrawAndEditValueMenu(&D_801341D0, 0x17);
        result = gOvl30RecordMenuCursor;
    } else {
        result = arg0;
    }
    return result;
}
