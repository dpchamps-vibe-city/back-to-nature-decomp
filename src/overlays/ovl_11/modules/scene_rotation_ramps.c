/*
 * Flag-triggered rotation ramps for the scene's two marked records.
 * Scene setup selects the records; scene updates step them; event dispatch
 * queries the held phases. See ovl11_scene_rotation_ramps.h for timing and
 * angle units, and notes/file-groupings.md for membership/boundary evidence.
 */
#include "common.h"
#include "ovl11_scene_rotation_ramps.h"
#include "game_flags.h"

void Ovl11InitSceneZRotationRamp(void) {
    s32 index;
    Ovl11SceneRecordRotationView *record;

    gOvl11SceneZRotationRampState = 0;
    for (index = 0, record = (Ovl11SceneRecordRotationView *)gOvl11SceneRecords;
         index < 15; index++, record++) {
        if (record->flags & 0x40) {
            record->rotationX = 0;
            record->rotationY = 0;
            record->rotationZ = 0;
            gOvl11SceneZRotationRecord = record;
            return;
        }
    }
    gOvl11SceneZRotationRecord = 0;
}

void Ovl11StepSceneZRotationRamp(void) {
    s32 flag;

    if (gOvl11SceneZRotationRecord == 0) {
        return;
    }
    if (gOvl11SceneZRotationRampState == 1) {
        goto outward;
    }
    if (gOvl11SceneZRotationRampState == 0) {
        goto idle;
    }
    if (gOvl11SceneZRotationRampState == 2) {
        goto held;
    }
    if (gOvl11SceneZRotationRampState == 3) {
        goto returning;
    }
    return;
idle:
    flag = GetGameFlag(0x71U);
    if (flag == 1) {
        gOvl11SceneZRotationRampState = flag;
        return;
    }
    return;
outward:
    gOvl11SceneZRotationRecord->rotationZ += 0x20;
    if (gOvl11SceneZRotationRecord->rotationZ >= 0x280) {
        gOvl11SceneZRotationRecord->rotationZ = 0x280;
        gOvl11SceneZRotationRampState = 2;
        return;
    }
    return;
held:
    if (GetGameFlag(0x71U) == 0) {
        gOvl11SceneZRotationRampState = 3;
        return;
    }
    return;
returning:
    gOvl11SceneZRotationRecord->rotationZ -= 0x20;
    if (gOvl11SceneZRotationRecord->rotationZ <= 0) {
        gOvl11SceneZRotationRecord->rotationZ = 0;
        gOvl11SceneZRotationRampState = 0;
    }
}

s32 Ovl11IsSceneZRotationRampHeld(void) {
    return gOvl11SceneZRotationRampState == 2;
}

void Ovl11InitSceneYRotationRamp(void) {
    s32 index;
    Ovl11SceneRecordRotationView *record;

    gOvl11SceneYRotationRampState = 0;
    for (index = 0, record = (Ovl11SceneRecordRotationView *)gOvl11SceneRecords;
         index < 15; index++, record++) {
        if (record->flags & 0x20) {
            record->rotationX = 0;
            record->rotationY = 0;
            record->rotationZ = 0;
            gOvl11SceneYRotationRecord = record;
            return;
        }
    }
    gOvl11SceneYRotationRecord = 0;
}

void Ovl11StepSceneYRotationRamp(void) {
    s32 flag;

    if (gOvl11SceneYRotationRecord == 0) {
        return;
    }
    if (gOvl11SceneYRotationRampState == 1) {
        goto outward;
    }
    if (gOvl11SceneYRotationRampState == 0) {
        goto idle;
    }
    if (gOvl11SceneYRotationRampState == 2) {
        goto held;
    }
    if (gOvl11SceneYRotationRampState == 3) {
        goto returning;
    }
    return;
idle:
    flag = GetGameFlag(0x70U);
    if (flag == 1) {
        gOvl11SceneYRotationRampState = flag;
    }
    return;
outward:
    gOvl11SceneYRotationRecord->rotationY -= 0x20;
    if (gOvl11SceneYRotationRecord->rotationY < -0x1BF) {
        gOvl11SceneYRotationRecord->rotationY = -0x1C0;
        gOvl11SceneYRotationRampState = 2;
    }
    return;
held:
    if (GetGameFlag(0x70U) == 0) {
        gOvl11SceneYRotationRampState = 3;
    }
    return;
returning:
    gOvl11SceneYRotationRecord->rotationY += 0x20;
    if (gOvl11SceneYRotationRecord->rotationY >= 0) {
        gOvl11SceneYRotationRecord->rotationY = 0;
        gOvl11SceneYRotationRampState = 0;
    }
}

s32 Ovl11IsSceneYRotationRampHeld(void) {
    return gOvl11SceneYRotationRampState == 2;
}
