#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

s32 ovl_17_func_800BB094(Recon_ovl_17_func_800BB094_A0View *arg0) {
    if (arg0->unk24 == 1) {
        return 1;
    } else {
        SelectSpriteAnimation(((void *)(((s32)arg0) + 0x20)), 1);
    }
}
