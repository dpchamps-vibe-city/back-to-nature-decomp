#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "game_flags.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "ovl11_record_flag_counters.h"
#include "ovl11_progress_gate.h"


s32 ovl_11_func_800EF0E8 (s16 arg0);
void ovl_11_func_800EF488 (s16 arg0);
s32 ovl_11_func_800EF8BC (s16 arg0, s16 arg1, s16 arg2);

void ovl_11_func_800EEECC(void) {
    s16 var_s0;

    SetGameFlag(0x26U, 0U);
    SetGameFlag(0x27U, 0U);
    SetGameFlag(0x28U, 0U);
    SetGameFlag(0x29U, 0U);
    SetGameFlag(0x2AU, 0U);
    for (var_s0 = 0; var_s0 < 5; var_s0++) {
        if (ovl_11_func_800EF0E8(var_s0) == 1) {
            ovl_11_func_800EF488(var_s0);
        }
    }
    if (((struct Ovl11ButtonUpdateState *) D_8006C838)->field_44BA == 0 &&
        ((struct Ovl11ButtonUpdateState *) D_8006C838)->field_44BC == 7 &&
        GetGameFlag(0x1BU) == 0) {
        ((struct Ovl11ButtonUpdateState *) D_8006C838)->field_E668 = 0;
    }
}
