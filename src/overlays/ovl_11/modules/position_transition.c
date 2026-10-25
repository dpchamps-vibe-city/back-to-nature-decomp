/*
 * Script-selected position transition, ovl_11:800CE210..800CE514.
 * Owns the endpoint/step protocol; see ovl11_position_transition.h for the
 * public state effects. Membership and evidence-limited cuts are recorded
 * in notes/file-groupings.md. The two Y paths deliberately differ: the
 * shared output follows a clamped linear sample, the object a cumulative
 * displacement about step 8.
 */
#include "ovl11_position_transition.h"

void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8001AF70(u16 arg0, u16 arg1);
void func_8001F278(s32 step, s32 length, s32 *start, s32 *end, s32 *out);
void Ovl11SelectPositionTransitionTarget(s32 *out);

void Ovl11BeginPositionTransition(Ovl11C7010Object *object) {
    object->unk38 = 0;
    object->unk6C |= 0x20001800;
    ovl_11_func_800C70CC(object, 0x4AU, 0, -2);
    func_8001AF70(9U, 1U);
    Ovl11SelectPositionTransitionTarget(&gOvl11PositionTransitionTarget.x);
    gOvl11PositionTransitionStart.x = ((Ovl11PositionTransitionObject *)object)->x;
    gOvl11PositionTransitionStart.y = ((Ovl11PositionTransitionObject *)object)->y;
    gOvl11PositionTransitionStart.z = ((Ovl11PositionTransitionObject *)object)->z;
    gOvl11PositionTransitionTarget.x -= 0x226;
    gOvl11PositionTransitionTarget.y = gOvl11PositionTransitionStart.y;
    gOvl11PositionTransitionTarget.z += 0x1B8;
    ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x400000;
    gOvl11PositionTransitionStep = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x4000;
}

/* Script code chooses a row, with all unlisted codes falling back to row 0.
 * The sole caller uses only the three-word destination, not a return value. */
void Ovl11SelectPositionTransitionTarget(s32 *out) {
    char *work = (char *)&D_8007AFF0;
    s32 *script = *(s32 **)(work + 0x25388);
    s32 row;
    s32 x;
    s32 z;

    switch (script[1]) {
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    default:
        row = 0;
        break;
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:
        row = 1;
        break;
    case 0x58:
    case 0x59:
    case 0x5A:
    case 0x5B:
        row = 2;
        break;
    }
    x = gOvl11PositionTransitionTargets[row][0];
    out[1] = 0;
    out[0] = x;
    z = gOvl11PositionTransitionTargets[row][1];
    out[2] = z;
}

s32 Ovl11StepPositionTransition(Ovl11PositionTransitionObject *object) {
    s32 objectYLimit;
    s32 displacement;
    s32 objectY;
    s32 distanceFromMidpoint;
    s32 outputY;
    Ovl11PositionTransitionSample sample;
    char *work;

    func_8001F278(gOvl11PositionTransitionStep, 0x11, (s32 *)&gOvl11PositionTransitionStart, (s32 *)&gOvl11PositionTransitionTarget, &sample.x);
    work = (char *)&D_8007AFF0;
    outputY = gOvl11PositionTransitionStart.y - 0x28;
    (*(u16 *)(work + 0x253B4)) = (u16)sample.x;
    (*(u16 *)(work + 0x253B8)) = (u16)sample.z;
    if (outputY < sample.y) {
        outputY = sample.y;
    }
    (*(s16 *)(work + 0x253B6)) = (s16)outputY;
    if (gOvl11PositionTransitionStep < 9) {
        distanceFromMidpoint = 8 - gOvl11PositionTransitionStep;
    } else {
        distanceFromMidpoint = gOvl11PositionTransitionStep - 8;
    }
    displacement = distanceFromMidpoint * 0x6F;
    displacement = displacement / 8;
    if (gOvl11PositionTransitionStep < 9) {
        objectY = object->y - displacement;
    } else {
        objectY = object->y + displacement;
    }
    objectYLimit = gOvl11PositionTransitionStart.y - 0x28;
    if (objectY >= objectYLimit) {
        objectY = objectYLimit;
    }
    if (gOvl11PositionTransitionStep >= 0x11) {
        ((struct struct_8006C838_view *)D_8006C838)->field_0C = ((struct struct_8006C838_view *)D_8006C838)->field_0C & 0xFFBFFFFF;
        object->x = sample.x;
        object->y = gOvl11PositionTransitionStart.y - 0x28;
        object->z = sample.z;
        return 1;
    }
    object->y = objectY;
    object->x = sample.x;
    object->z = sample.z;
    gOvl11PositionTransitionStep += 1;
    return 0;
}
