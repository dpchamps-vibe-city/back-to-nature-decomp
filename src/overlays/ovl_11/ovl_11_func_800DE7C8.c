#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_object_render.h"

s32 ovl_11_func_800DF3BC(u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);

void ovl_11_func_800DE7C8(u8 *arg0) {
    u16 sp10;
    u16 sp12;
    u16 temp_v1;

    ovl_11_func_800DF3BC(*(u16 *) ((u8 *) arg0 + 0x24), *(u16 *) ((u8 *) arg0 + 0x22), &sp10, &sp12);
    if (sp10 != (*(u8 *) ((u8 *) arg0 + 0x7C))) {
        SelectSpriteAnimation((ObjectState *) (arg0 + 0x78), (u8) sp10);
    }
    if ((sp12 != 0) && (sp12 != (*(u8 *) ((u8 *) arg0 + 0x7D)))) {
        SelectSpriteAnimationFrame((ObjectState *) (arg0 + 0x78), (u8) sp12);
    }
    Ovl11DrawObjectWithMarker(arg0);
    temp_v1 = *(u16 *) ((u8 *) arg0 + 0x24);
    if ((temp_v1 == 0) || (temp_v1 == 5)) {
        (*(u16 *) ((u8 *) arg0 + 0x7A)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0x7A)) | 0x100);
    }
}
