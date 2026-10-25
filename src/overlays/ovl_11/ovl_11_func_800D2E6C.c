#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_object_render.h"

s32 ovl_11_func_800D3424(u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);

void ovl_11_func_800D2E6C(u8 *arg0) {
    u16 animation;
    u16 subAnimation;

    ovl_11_func_800D3424(*(u16 *)(arg0 + 0x24), *(u16 *)(arg0 + 0x22),
                        &animation, &subAnimation);
    if (animation != arg0[0x7C]) {
        SelectSpriteAnimation((ObjectState *)(arg0 + 0x78), (u8)animation);
    }
    if (subAnimation != 0 && subAnimation != arg0[0x7D]) {
        SelectSpriteAnimationFrame((ObjectState *)(arg0 + 0x78), (u8)subAnimation);
    }
    SetSpriteAnimationUpdateLatch(0);
    Ovl11DrawObjectWithMarker(arg0);
    if (*(u16 *)(arg0 + 0x24) == 0) {
        *(u16 *)(arg0 + 0x7A) |= 0x100;
    }
}
