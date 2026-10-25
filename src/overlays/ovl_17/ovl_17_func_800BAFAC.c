#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

s32 ovl_17_func_800BAFAC(Recon_ovl_17_func_800BAFAC_A0View *arg0) {
    if (arg0->unk24 == 2) {
        return 2;
    } else {
        SelectSpriteAnimation(((void *)(((s32)arg0) + 0x20)), 2);
    }
}
