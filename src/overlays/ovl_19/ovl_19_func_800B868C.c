#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "packet_work.h"
#include "ovl19_value_table_display.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"


void ovl_19_func_800BA770 (void);
void ovl_19_func_800BA834 (s16 *arg0);
void ovl_19_func_800BAB4C (Ovl19Func800BAB4CArg *arg0);
void ovl_19_func_800BAFAC (s32 arg0, ObjectState *arg1, s16 *arg2, s16 arg3);
u32 func_80017A64 (void);
void func_80017A48 (u32 arg0);
s32 ovl_19_func_800BADAC (s16 arg0);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_19_func_800B868C(void) {
    ovl_19_func_800BA770();
    Ovl19DrawValueTable();
    if ((*(s32 *) ((u8 *) gPad0Input + 8)) & 0x60) {
        D_800BF4C4 = 1;
    }
}
/* Warning: struct GfxObj is not defined (only forward-declared) */
