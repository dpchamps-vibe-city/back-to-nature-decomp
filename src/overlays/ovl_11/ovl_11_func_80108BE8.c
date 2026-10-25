#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

s32 ovl_11_func_80109550(u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);
/* Engine update-latch setter, exe:0x800158D8. */
void ovl_11_func_800D0198(u8 *arg0, s32 arg1, s32 arg2);

void ovl_11_func_80108BE8(u8 *arg0) {
    u16 animation;
    u16 subAnimation;

    if (!(D_8006C844 & 0x800000)) {
        ovl_11_func_80109550(*(u16 *)(arg0 + 0x24), *(u16 *)(arg0 + 0x22),
                            &animation, &subAnimation);
        if (animation != arg0[0x7C]) {
            SelectSpriteAnimation((ObjectState *)(arg0 + 0x78), (u8)animation);
        }
        if (subAnimation != 0 && subAnimation != arg0[0x7D]) {
            SelectSpriteAnimationFrame((ObjectState *)(arg0 + 0x78), (u8)subAnimation);
        }
        SetSpriteAnimationUpdateLatch(0);
        ovl_11_func_800D0198(arg0, (s32)(arg0 + 0xE0 + gActiveRenderContextIndex * 8), 0x2800);
    }
}
