#include "common.h"
#include "ovl11_object_render.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"

void ovl_11_func_8010C2EC(u8 *arg0) {
    if (!(D_8006C844 & 0x800000)) {
        Ovl11DrawObjectWithMarker(arg0);
        Ovl11ResetTimedSpriteEffect((s16 *)(arg0 + 0xA8));
    }
}
