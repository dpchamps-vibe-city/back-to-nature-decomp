#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

s32 ovl_11_func_800D3424(u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);
/* Engine update-latch setter, exe:0x800158D8. */
void ovl_11_func_800CFE40(u8 *arg0);

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
    ovl_11_func_800CFE40(arg0);
    if (*(u16 *)(arg0 + 0x24) == 0) {
        *(u16 *)(arg0 + 0x7A) |= 0x100;
    }
}
