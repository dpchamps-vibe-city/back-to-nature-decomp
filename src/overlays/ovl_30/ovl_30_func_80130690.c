#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"
#include "ovl30_record_menus.h"

s32 ovl_30_func_80130690(s32 arg0) {
    s32 result;

    if (IsScreenFadeComplete() != 0) {
        KanjiFntPrint(D_8012E848);
        gOvl30RecordMenuCursor = arg0;
        Ovl30DrawAndEditValueMenu(&D_801345E0, 0x30);
        result = gOvl30RecordMenuCursor;
    } else {
        result = arg0;
    }
    return result;
}
