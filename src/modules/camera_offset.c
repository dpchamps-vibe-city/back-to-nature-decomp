/*
 * Cylindrical camera-offset state and coordinate conversion.
 *
 * Parameters are radius, height and a 12-bit yaw, not viewport dimensions:
 * conversion writes (cos(yaw)*radius, -height, sin(yaw)*radius). Setters
 * notify the retained change machinery; only initialization computes the
 * vector here. The three unused entry points remain at their original
 * positions and share this unit's small-data state; the readback, the guarded
 * target update and the mode-word setter are C.
 *
 * Public contracts: camera_offset.h. Membership/boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "camera_offset.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "vector_ops.h"

/* Camera-offset state words; this unit owns them, so they are tentative
 * definitions here and GP-relative accesses resolve inside the file. */
u8 gCameraOffsetState;
s8 gCameraOffsetInitialized;
s8 gCameraOffsetDirty;
/* Last mode word accepted by func_8001F1C0; it sits between the dirty byte and
 * the change-state word in this unit's small-data block. */
s32 D_8005E2F0;
s32 gCameraOffsetChangeState;
s32 gCameraOffsetRadius;
s32 gCameraOffsetHeight;
s32 gCameraOffsetYaw;
/* Lock word read by func_8001F100 before it accepts a new offset target. */
s32 D_8005E304;
/* Auxiliary initialization words; their later roles are not established. */
s32 D_8005E308;
s32 D_8005E30C;
s32 D_8005E310;
s32 D_8005E314;

void InitCameraOffsetState(void) {
    s32 *offset;
    Vec3 *origin;

    SetCameraOffsetParameters(0x320, 0x258, 0);

    offset = &gCameraOffsetVector;
    origin = &gCameraOffsetOrigin;
    D_8005E308 = -1;
    D_8005E30C = 0;
    D_8005E310 = 0;
    D_8005E314 = 0;
    gCameraOffsetDirty = 0;
    origin->z = 0;
    origin->y = 0;
    origin->x = 0;

    ComputeCylindricalOffset(offset, gCameraOffsetRadius, gCameraOffsetHeight, gCameraOffsetYaw);
    CopyVec3(&gCameraOffsetBackup, (Vec3 *)offset);

    gCameraOffsetInitialized = 1;
    gCameraOffsetState = 2;
}

void SetCameraOffsetParameters(s32 radius, s32 height, s32 yaw) {
    if ((radius != gCameraOffsetRadius) || (height != gCameraOffsetHeight) || (yaw != gCameraOffsetYaw)) {
        gCameraOffsetDirty = 1;
        gCameraOffsetChangeState = 0;
    }
    gCameraOffsetRadius = radius;
    gCameraOffsetHeight = height;
    gCameraOffsetYaw = yaw;
}

/* Read back the current parameters into the caller's words. When an angle word
 * is supplied, also return atan2(z, x) of the offset minus the origin. Every
 * destination is optional and a null pointer skips that output. */
void func_8001F084(s32 *radiusOut, s32 *heightOut, s32 *angleOut) {
    Vec3 delta;

    if (radiusOut != NULL) {
        *radiusOut = gCameraOffsetRadius;
    }
    if (heightOut != NULL) {
        *heightOut = gCameraOffsetHeight;
    }
    if (angleOut != NULL) {
        CopyVec3(&delta, (Vec3 *)&gCameraOffsetVector);
        SubtractVec3(&delta, (Vec3 *)&gCameraOffsetOrigin);
        *angleOut = ratan2(delta.z, delta.x);
    }
}

/* Unused target-change notification: accept a new offset target only while the
 * state is active, the target really differs from the stored triple, and the
 * lock word does not hold it. Accepting copies the triple and marks the state
 * dirty with a reset change entry. */
void func_8001F100(Vec3 *target) {
    if (gCameraOffsetState != 2) {
        return;
    }
    if ((target->x == D_8004928C[0]) && (target->y == D_8004928C[1]) && (target->z == D_8004928C[2])) {
        return;
    }
    if (D_8005E304 == 1) {
        return;
    }
    CopyVec3((Vec3 *)D_8004928C, target);
    gCameraOffsetDirty = 1;
    gCameraOffsetChangeState = 0;
}

void CopyCameraOffsetOrigin(s32 destination) {
    CopyVec3((Vec3 *)destination, (Vec3 *)&gCameraOffsetOrigin);
}

void ClearCameraOffsetState(void) {
    gCameraOffsetState = 0;
}

/* Unused mode-word setter: record the new mode and, when it differs from the
 * stored one, mark the same dirty byte the parameter setters mark. */
void func_8001F1C0(s32 mode) {
    if (D_8005E2F0 != mode) {
        gCameraOffsetDirty = 1;
    }
    D_8005E2F0 = mode;
}

void ComputeCylindricalOffset(s32 *out, s32 radius, s32 height, s32 yaw) {
    s32 angle;
    s32 negativeHeight;

    angle = yaw & 0xFFF;
    negativeHeight = -height;
    out[1] = negativeHeight;
    out[0] = (rcos(angle) * radius) >> 12;
    out[2] = (rsin(angle) * radius) >> 12;
}
