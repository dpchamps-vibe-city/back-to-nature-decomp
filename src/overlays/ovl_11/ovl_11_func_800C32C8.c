#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "random.h"
#include "game_flags.h"

#define STATE ((struct Ovl11AnimationState *) D_8006C838)

void ovl_11_func_800C32C8(Ovl11AnimationObject *obj, s32 arg1, s32 arg2, s32 arg3) {
    ObjectState *initial = (ObjectState *) &obj->sprite;
    SpriteSourceData *sprite;
    u16 hour;
    s16 allowed;
    u16 previous;
    s16 id;
    u16 *flags = &obj->flags;

    previous = obj->field_1A;
    obj->field_1A = arg1;
    obj->field_8 = arg3;
    *flags |= 0x80;
    obj->field_1C = previous;
    SelectSpriteAnimation(initial, arg2);
    if (STATE->selected == obj->id) {
        if (GetGameFlag(0x4F) == 1 && STATE->mode == 0) {
            hour = (u16) STATE->hour;
            allowed = (hour < 5 || hour >= 12);
            if (hour >= 14 && hour < 16) {
                allowed = 0;
            }
            if (allowed && arg2 < 5) {
                SelectSpriteAnimation(initial, arg2 + 5);
            }
        }
        if (STATE->hour >= 16) {
            if (STATE->field_44CE == 2) {
                SetGameFlag(0x72, 1);
            }
            SelectSpriteAnimation((ObjectState *) &obj->sprite, 10);
            return;
        }
    }
    id = obj->id;
    sprite = &obj->sprite;
    if (id == 0x24) {
        if (GetGameFlag(0x4F) == 1) {
            if (STATE->mode == 0) {
                s32 value;

                obj->field_6 = 0;
                value = Rand(2);
                if (value != 0) {
                    SelectSpriteAnimation((ObjectState *) sprite, 2);
                } else {
                    SelectSpriteAnimation((ObjectState *) sprite, 4);
                }
                if (STATE->hour >= 14) {
                    SelectSpriteAnimation((ObjectState *) sprite, 5);
                    obj->field_6 = 0;
                }
            } else {
                SelectSpriteAnimation((ObjectState *) sprite, (u8) obj->field_6 + 10);
                if (STATE->hour >= 12) {
                    SelectSpriteAnimation((ObjectState *) sprite, 5);
                    obj->field_6 = 0;
                }
            }
        }
    }
    if (obj->id == 0x18 && ((struct Ovl11SpriteWork *) &D_8007AFF0)->header[1] == 1 && obj->field_E == 4) {
        SelectSpriteAnimation((ObjectState *) sprite, (u8) obj->field_6 + 5);
    }
    if (GetSpriteFrameControlHighBit(sprite) != 0) {
        obj->sprite.field_5 = *(u8 *) &obj->field_6;
    }
    if (obj->id == 0x17) {
        SelectSpriteAnimation((ObjectState *) sprite, 1);
    }
}
