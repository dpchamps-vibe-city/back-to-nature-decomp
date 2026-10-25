#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

extern s32 D_80123154;
extern s16 gCalendarSeason;

/* Callee prototypes as the original caller TU saw them. */
s32 ovl_11_func_800D688C(s16 arg0);

void ovl_11_func_800CE5FC(void) {
    s32 header;

    D_80123154 = 0;
    if (gCalendarSeason == 3) {
        header = ovl_11_func_800D688C(0x187);
        if (header != 0) {
            D_80123154 = 1;
            InitSpriteSourceData(&D_80128D00, (SpriteDataHeader *)header);
        }
    }
}
