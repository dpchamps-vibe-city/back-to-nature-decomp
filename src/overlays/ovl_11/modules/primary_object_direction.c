/*
 * Held-button facing selection and direction/mode-indexed X/Z offsets.
 * The two helpers form the update's facing -> table index -> offset chain.
 * See ovl11_primary_object_direction.h for inputs, effects and return value;
 * notes/file-groupings.md records the evidence-limited module boundaries.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_primary_object_direction.h"
#include "ovl11_primary_object_state_change.h"
#include "ovl11_facing_history.h"

s32 Ovl11MapDirectionButtons(u16 buttons, u16 facing) {
    switch (buttons) {
    case 0x4000:
    case 0xC000:
        facing = 0;
        break;
    case 0x8000:
    case 0x9000:
        facing = 1;
        break;
    case 0x1000:
    case 0x3000:
        facing = 2;
        break;
    case 0x2000:
    case 0x6000:
        facing = 3;
        break;
    }
    return facing;
}

s32 Ovl11AccumulateFacingOffset(Ovl11FacingOffsetObject *object) {
    Ovl11DirectionOffset *offset;

    offset = (Ovl11DirectionOffset *)Ovl11CollisionOffsets;
    if (object->state == 2) {
        offset += 4;
    }
    if (object->state == 4) {
        offset = gOvl11DirectionOffsetsMode4;
    }
    if (object->flags & 0x100) {
        if (object->state == 2) {
            offset = gOvl11DirectionOffsetsMode4;
        } else {
            offset = gOvl11DirectionOffsetsFlag100;
        }
    }
    offset += object->facing;
    object->offsetX += offset->x;
    object->offsetZ += offset->z;
    return object->offsetZ;
}

s32 Ovl11UpdatePrimaryObjectDirection(void) {
    Ovl11C7010Object *object;
    s32 actionModifier;
    u16 oldFacing;
    u16 directionButtons;

    object = (Ovl11C7010Object *) &gPrimaryObjectBase;
    actionModifier = 0;
    directionButtons = *(u16 *) &gOvl11PrimaryHeldButtons;
    oldFacing = object->unk38;
    if (directionButtons & 0x20) {
        actionModifier = 1;
        if (object->unk3C == 4) {
            actionModifier = 2;
        }
    }
    directionButtons &= 0xF000;
    Ovl11PushFacingHistory((s32) (s16) object->unk38);
    object->unk38 = Ovl11MapDirectionButtons(directionButtons, object->unk38);
    if ((object->unk6C & 0x200000) && (actionModifier != 0)) {
        actionModifier = 0;
        directionButtons = 1;
    }
    if (directionButtons == 0) {
        if (actionModifier != 0) {
            if (object->unk38 != oldFacing || actionModifier == 1) {
                Ovl11ChangePrimaryObjectState(object, 4U, gPrimaryObjectFacing + 6, -2);
            }
            object->unk6C = (s32) (object->unk6C & 0xDFFFFFFF);
            Ovl11AccumulateFacingOffset((Ovl11FacingOffsetObject *) &gPrimaryObjectBase);
            return 1;
        }
        Ovl11ChangePrimaryObjectState(object, 1U, 0, -2);
        object->unk6C = (s32) ((object->unk6C & 0xDFFFFFFF) | 0x04000000);
        return 0;
    }
    Ovl11AccumulateFacingOffset((Ovl11FacingOffsetObject *) &gPrimaryObjectBase);
    if (actionModifier != 0) {
        if ((object->unk38 != oldFacing) || (object->unk3C != 4)) {
            Ovl11ChangePrimaryObjectState(object, 4, ((Ovl11C7010Object *) &gPrimaryObjectBase)->unk38 + 6, -2);
            object->unk6C &= 0xDFFFFFFF;
        }
    } else if ((object->unk38 != oldFacing) || (object->unk3C != 2)) {
        Ovl11ChangePrimaryObjectState(object, 2, ((Ovl11C7010Object *) &gPrimaryObjectBase)->unk38 + 1, -2);
        object->unk6C &= 0xDFFFFFFF;
    }
    return 1;
}
