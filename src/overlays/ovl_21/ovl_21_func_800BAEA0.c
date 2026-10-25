#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

void ovl_21_func_800BAFFC();

void ovl_21_func_800BAEA0(s32 *arg0) {
    if ((*(s16 *) ((u8 *) arg0 + 4)) != (*(u8 *) ((u8 *) arg0 + 0x20))) {
        SelectSpriteAnimation((ObjectState *) (arg0 + 7), *(u8 *) ((u8 *) arg0 + 4));
    }
    ovl_21_func_800BAFFC((SpriteSourceData *) (arg0 + 7), (u16 *) (arg0 + 3));
}
