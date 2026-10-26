#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_object_render.h"
#include "ovl11_item_sprite_source.h"

s32 ovl_11_func_8010D20C(u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);

void ovl_11_func_8010CA0C(Ovl11ItemSpriteObject *arg0) {
    u16 sp10;
    u16 sp12;
    u16 temp_v1;

    ovl_11_func_8010D20C(*(u16 *) ((u8 *) arg0 + 0x24), *(u16 *) ((u8 *) arg0 + 0x22), &sp10, &sp12);
    if (sp10 != (*(u8 *) ((u8 *) arg0 + 0x7C))) {
        SelectSpriteAnimation((ObjectState *) ((u8 *) arg0 + 0x78), (u8) sp10);
    }
    temp_v1 = *(u16 *) ((u8 *) arg0 + 0x24);
    if (((temp_v1 == 0) || (temp_v1 == 0xF)) && (sp12 != (*(u8 *) ((u8 *) arg0 + 0x7D)))) {
        SelectSpriteAnimationFrame((ObjectState *) ((u8 *) arg0 + 0x78), (u8) sp12);
    }
    Ovl11InstallItemSpriteSource(arg0);
    Ovl11DrawObjectWithExtraSprite((u8 *) arg0);
}
