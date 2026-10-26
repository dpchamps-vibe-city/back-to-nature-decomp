#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"

s32 ovl_11_func_800E276C(s16 *arg0);
void ovl_11_func_800E1F70(s32 arg0);
void ovl_11_func_800E1EC0(u8 *arg0);

void ovl_11_func_800D2160(void) {
    s16 *entry;
    s32 i;
    u8 *far_base;
    u8 *base;

    for (i = 0; i < 3; i++) {
        entry = (s16 *)((u8 *)&D_80075FE4 + i * 0xB4);
        far_base = D_8009AFF0;
        base = (u8 *)D_8006C838;
        if (*(u16 *)entry != 0) {
            ovl_11_func_800E276C(entry);
            if (*(s16 *)(far_base + 0x5476) != *(s16 *)((u8 *)entry + 0x30)) {
                Ovl11ResetTimedSpriteEffect(entry + 0x54);
            } else {
                if (!(*(s32 *)(base + 0xC) & 0x08000000)) {
                    ovl_11_func_800E1F70((s32)entry);
                }
                ovl_11_func_800E1EC0((u8 *)entry);
                Ovl11TickTimedSpriteEffect((Ov11Timer *)(entry + 0x54));
            }
        }
    }
}
