#include "common.h"
#include "game_types.h"

/* Byte-valued animation index: the callee stores it with sb, and this
 * target masks all computed indices to 0xFF before the call. */
void func_80015840(ObjectState *obj, u8 arg1);
u32 func_8001589C(SpriteSourceData *src);
s32 func_8001AF44(u32 arg0);
void func_8001AF70(u16 arg0, u16 arg1);
/* Overlay import of the matched Rand at 0x80012A34. */
u32 Rand(s32 arg0);

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
    func_80015840(initial, arg2);
    if (STATE->selected == obj->id) {
        if (func_8001AF44(0x4F) == 1 && STATE->mode == 0) {
            hour = (u16) STATE->hour;
            allowed = (hour < 5 || hour >= 12);
            if (hour >= 14 && hour < 16) {
                allowed = 0;
            }
            if (allowed && arg2 < 5) {
                func_80015840(initial, arg2 + 5);
            }
        }
        if (STATE->hour >= 16) {
            if (STATE->field_44CE == 2) {
                func_8001AF70(0x72, 1);
            }
            func_80015840((ObjectState *) &obj->sprite, 10);
            return;
        }
    }
    id = obj->id;
    sprite = &obj->sprite;
    if (id == 0x24) {
        if (func_8001AF44(0x4F) == 1) {
            if (STATE->mode == 0) {
                s32 value;

                obj->field_6 = 0;
                value = Rand(2);
                if (value != 0) {
                    func_80015840((ObjectState *) sprite, 2);
                } else {
                    func_80015840((ObjectState *) sprite, 4);
                }
                if (STATE->hour >= 14) {
                    func_80015840((ObjectState *) sprite, 5);
                    obj->field_6 = 0;
                }
            } else {
                func_80015840((ObjectState *) sprite, (u8) obj->field_6 + 10);
                if (STATE->hour >= 12) {
                    func_80015840((ObjectState *) sprite, 5);
                    obj->field_6 = 0;
                }
            }
        }
    }
    if (obj->id == 0x18 && ((struct Ovl11SpriteWork *) &D_8007AFF0)->header[1] == 1 && obj->field_E == 4) {
        func_80015840((ObjectState *) sprite, (u8) obj->field_6 + 5);
    }
    if (func_8001589C(sprite) != 0) {
        obj->sprite.field_5 = *(u8 *) &obj->field_6;
    }
    if (obj->id == 0x17) {
        func_80015840((ObjectState *) sprite, 1);
    }
}
