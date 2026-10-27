/*
 * object_sprite_animation_command — primary sprite hold/frame/deferred control.
 * The shared source/record views retain their original field widths.
 * Membership/boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "sprite_source.h"
#include "ovl11_object_sprite_animation_command.h"

s32 Ovl11ObjectSpriteAnimationCommand(s16 mode, s16 animationId, s32 frameOrReuse, s32 operand3) {
    s16 selectedFrame;
    s32 control = operand3;
    ObjectState *object = (ObjectState *) gPrimaryObjectSpriteSource;

    if (frameOrReuse == -1) {
        selectedFrame = *(s16 *) ((u8 *) object - 0x258);
    } else {
        selectedFrame = frameOrReuse;
    }
    if (mode == -1) {
        object->field_4 = 0;
        ((struct Ovl11AnimationCommandWorkView *) &D_8006C838)->flags &= ~0x100000;
        goto ret1;
    }
    if (mode == 1) {
        ((struct Ovl11AnimationCommandWorkView *) &D_8006C838)->flags |= 0x100000;
        if (control == 0) {
            if ((u8) object->field_4 != animationId) {
                SelectSpriteAnimation(object, animationId & 0xFF);
                return 0;
            }
            if ((object->field_2 & 0x300) != 0) {
                ((struct Ovl11AnimationCommandWorkView *) &D_8006C838)->flags &= ~0x100000;
                object->field_4 = 0;
                return 1;
            }
            return 0;
        }
        if ((u8) object->field_4 != animationId) {
            SelectSpriteAnimation(object, animationId & 0xFF);
        }
        SelectSpriteAnimationFrame(object, selectedFrame & 0xFF);
        return 1;
    }
    ((struct Ovl11AnimationCommandWorkView *) &D_8006C838)->deferredRecord = gOvl11DeferredAnimationRecord;
    ((struct Ovl11DeferredAnimationCommand *) gOvl11DeferredAnimationRecord)->animationId = animationId;
    ((struct Ovl11DeferredAnimationCommand *) gOvl11DeferredAnimationRecord)->frame = selectedFrame;
    ((struct Ovl11DeferredAnimationCommand *) gOvl11DeferredAnimationRecord)->adjustedOperand = (s16) (control - 0xA);
    ((struct Ovl11DeferredAnimationCommand *) gOvl11DeferredAnimationRecord)->value0C = 0x270F;
    ((struct Ovl11AnimationCommandWorkView *) &D_8006C838)->flags &= ~0x100000;
    ((struct Ovl11AnimationCommandWorkView *) &D_8006C838)->auxiliaryFlags |= 0x10;
    return 0;
ret1:
    return 1;
}
