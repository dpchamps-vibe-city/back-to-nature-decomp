/*
 * Cylindrical camera-offset state and coordinate conversion.
 *
 * Parameters are radius, height and a 12-bit yaw, not viewport dimensions:
 * conversion writes (cos(yaw)*radius, -height, sin(yaw)*radius). Setters
 * notify the retained change machinery; only initialization computes the
 * vector here. The three unused entry points remain at their original
 * positions and share this unit's small-data state.
 *
 * Public contracts: camera_offset.h. Membership/boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "camera_offset.h"
#include "include_asm.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "vector_ops.h"

/* Tentative definitions preserve the original unit's GP-relative accesses. */
s8 gCameraOffsetState;
s8 gCameraOffsetInitialized;
s8 gCameraOffsetDirty;
s32 gCameraOffsetChangeState;
s32 gCameraOffsetRadius;
s32 gCameraOffsetHeight;
s32 gCameraOffsetYaw;
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

/* Unused parameter readback, including atan2 of vector minus origin. */
INCLUDE_ASM("build/asm/nonmatchings/modules/camera_offset", func_8001F084);

/* Unused target-change notification, guarded by state and a lock word. */
INCLUDE_ASM("build/asm/nonmatchings/modules/camera_offset", func_8001F100);

void CopyCameraOffsetOrigin(s32 destination) {
    CopyVec3((Vec3 *)destination, (Vec3 *)&gCameraOffsetOrigin);
}

void ClearCameraOffsetState(void) {
    gCameraOffsetState = 0;
}

/* Unused mode-word setter; a change marks the same dirty byte. */
INCLUDE_ASM("build/asm/nonmatchings/modules/camera_offset", func_8001F1C0);

void ComputeCylindricalOffset(s32 *out, s32 radius, s32 height, s32 yaw) {
    s32 angle;
    s32 negativeHeight;

    angle = yaw & 0xFFF;
    negativeHeight = -height;
    out[1] = negativeHeight;
    out[0] = (rcos(angle) * radius) >> 12;
    out[2] = (rsin(angle) * radius) >> 12;
}
