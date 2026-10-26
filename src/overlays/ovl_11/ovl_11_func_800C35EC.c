#include "common.h"
#include "game_types.h"
#include "random.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"

void ovl_11_func_800C32C8(Ovl11AnimationObject *obj, s32 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800C35EC(Ovl11AnimationObject *obj) {
    s32 threshold;

    if (obj->field_1A == 3) {
        return 0;
    }
    if (obj->flags & 1) {
        return 0;
    }
    if (obj->flags & 0x1000) {
        return 0;
    }
    if (!(obj->flags & 0x100)) {
        return 0;
    }
    if (*(s16 *) ((u8 *) obj + 0xA) < 30) {
        threshold = *(s16 *) ((u8 *) obj + 2) - *(s16 *) ((u8 *) obj + 0xA) + 20;
        if ((s32) Rand(101) < threshold) {
            ovl_11_func_800C32C8(obj, 3, 0, 150);
            Ovl11StartTimedSpriteEffect((Ovl11C35ECStatePair *) ((u8 *) obj + 0xE0), 0x22, 150);
            obj->field_6 = (D_80071A38 + 2) & 3;
            obj->sprite.field_5 = *(u8 *) &obj->field_6;
            return 1;
        }
    }
    return 0;
}
