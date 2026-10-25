#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800C93A4(Ovl11C7010Object *arg0) {
    arg0->unk6C |= 0x1800;
    ovl_11_func_800C70CC(arg0, 0x3BU, 0, 0x14);
    (*(s32 *) ((u8 *) arg0 + 0x100)) = -0x2286;
    SelectSpriteAnimation((ObjectState *) &arg0->sprite, 4);
    arg0->unk38 = 2;
    (*(s32 *) ((u8 *) arg0 + 0x114)) = -0x8C;
    arg0->sprite.field_5 = arg0->unk38;
    arg0->unk6C |= 0x40000000;
    PlaySoundEffect(6);
}
