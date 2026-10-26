#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"

s32 ovl_11_func_800D3048(u16 *arg0);
void ovl_11_func_800D2F1C(s32 arg0);
void ovl_11_func_800D2E6C(u8 *arg0);

void ovl_11_func_800D2240(void) {
    s32 i;
    u16 *temp_s0;
    u8 *base;
    u8 *far_base;

    for (i = 0; i < 6; i++) {
        temp_s0 = (u16 *)(D_80075BC4_reset + i * 0xB0);
        base = (u8 *)D_8006C838;
        far_base = D_8009AFF0;
        if ((*temp_s0 != 0) && !((*(s32 *) ((u8 *) temp_s0 + 0x34)) & 0x20000)) {
            ovl_11_func_800D3048(temp_s0);
            if (!((*(s32 *) (base + 0xC)) & 0x08000000)) {
                ovl_11_func_800D2F1C((s32) temp_s0);
                Ovl11TickTimedSpriteEffect((Ov11Timer *) ((temp_s0 + 0x54)));
            }
            if ((*(s16 *) (far_base + 0x5476)) != (*(s16 *) ((u8 *) temp_s0 + 0x30))) {
                Ovl11ResetTimedSpriteEffect((s16 *) ((temp_s0 + 0x54)));
            } else {
                ovl_11_func_800D2E6C((u8 *) temp_s0);
            }
        }
    }
}
