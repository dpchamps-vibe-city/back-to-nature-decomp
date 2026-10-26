#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"

void ovl_11_func_800E46A8(u8 *arg0);

void ovl_11_func_800E44D4(void) {
    s32 var_s1;
    struct_80076220 *var_s0;

    if (!(D_8006C844 & 0x800000)) {
        for (var_s1 = 0; var_s1 < 37; var_s1++) {
            var_s0 = &(&gObjectEventSlotBase)[var_s1];
            if (((*(s32 *) ((u8 *) var_s0 + 0x1C)) & 0x18000000) == 0x18000000) {
                Ovl11TickTimedSpriteEffect((Ov11Timer *) ((u8 *) var_s0 + 0xE0));
                ovl_11_func_800E46A8(var_s0->unk0);
            }
        }
    }
}
