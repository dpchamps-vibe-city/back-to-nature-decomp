/*
 * Overlay-21 camera orbit and geometry drawing.
 *
 * Reset, update, apply and draw share the camera state in the secondary work
 * area. Geometry is loaded separately by the overlay's scene setup. The
 * related overlay-19 implementation is a copy, not the same function identity.
 * See ovl21_scene_camera_geometry.h and notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "gte_scene_state.h"
#include "projected_primitives.h"
#include "ovl21_scene_camera_geometry.h"

/* Offsets from D_8007AFF0. Word resets and low-halfword reads intentionally
 * retain their original widths; computed eye coordinates wrap to halfwords.
 * The two reset-only words at +2539C/+253A4 remain unnamed. */
enum {
    CAMERA_DISTANCE = 0x25394,
    CAMERA_HEIGHT = 0x25398,
    CAMERA_YAW = 0x253A0,
    CAMERA_OFFSET_X = 0x253AC,
    CAMERA_EYE_Y = 0x253AE,
    CAMERA_OFFSET_Z = 0x253B0,
    CAMERA_TARGET_X = 0x253B4,
    CAMERA_TARGET_Y = 0x253B6,
    CAMERA_TARGET_Z = 0x253B8
};

void Ovl21InitSceneCamera(void) {
    char *camera = (char *)&D_8007AFF0;

    *(s32 *)(camera + CAMERA_DISTANCE) = -0x2328;
    *(s32 *)(camera + CAMERA_HEIGHT) = -0x1194;
    *(s32 *)(camera + 0x2539C) = 0;
    *(s32 *)(camera + CAMERA_YAW) = -0x200;
    *(s32 *)(camera + 0x253A4) = 0;
    *(s16 *)(camera + CAMERA_TARGET_X) = 0;
    *(s16 *)(camera + CAMERA_TARGET_Y) = 0;
    *(s16 *)(camera + CAMERA_TARGET_Z) = 0;
}

void Ovl21UpdateSceneCameraOffset(void) {
    char *camera;
    MATRIX rotation;
    MATRIX identity;
    SVECTOR vector;
    VECTOR rotated;
    long flag;

    rotation = GsIDMATRIX;
    identity = GsIDMATRIX;
    PushMatrix();
    camera = (char *)&D_8007AFF0;
    vector.vx = 0;
    vector.vy = *(u16 *)(camera + CAMERA_YAW);
    vector.vz = 0;
    RotMatrix(&vector, &rotation);
    SetRotMatrix(&rotation);
    SetTransMatrix(&identity);
    vector.vx = 0;
    vector.vy = 0;
    vector.vz = *(u16 *)(camera + CAMERA_DISTANCE);
    RotTrans(&vector, &rotated, &flag);
    PopMatrix();
    *(u16 *)(camera + CAMERA_OFFSET_X) = rotated.vx;
    *(s16 *)(camera + CAMERA_EYE_Y) = *(u16 *)(camera + CAMERA_HEIGHT) + *(u16 *)(camera + CAMERA_TARGET_Y);
    *(u16 *)(camera + CAMERA_OFFSET_Z) = rotated.vz;
}

void Ovl21ApplySceneCamera(void) {
    char *camera = (char *)&D_8007AFF0;

    SetCameraEyeAndRoll(*(s16 *)(camera + CAMERA_OFFSET_X) + *(s16 *)(camera + CAMERA_TARGET_X),
                       *(s16 *)(camera + CAMERA_EYE_Y),
                       *(s16 *)(camera + CAMERA_OFFSET_Z) + *(s16 *)(camera + CAMERA_TARGET_Z),
                       0);
    SetCameraTarget(*(s16 *)(camera + CAMERA_TARGET_X),
                    *(s16 *)(camera + CAMERA_TARGET_Y),
                    *(s16 *)(camera + CAMERA_TARGET_Z));
}

void Ovl21DrawSceneGeometry(void) {
    char *camera;
    VECTOR eye;
    VECTOR target;

    camera = (char *)&D_8007AFF0;
    eye.vx = *(s16 *)(camera + CAMERA_OFFSET_X) + *(s16 *)(camera + CAMERA_TARGET_X);
    eye.vy = *(s16 *)(camera + CAMERA_EYE_Y);
    eye.vz = *(s16 *)(camera + CAMERA_OFFSET_Z) + *(s16 *)(camera + CAMERA_TARGET_Z);
    target.vx = *(s16 *)(camera + CAMERA_TARGET_X);
    target.vy = *(s16 *)(camera + CAMERA_TARGET_Y);
    target.vz = *(s16 *)(camera + CAMERA_TARGET_Z);
    DrawCulledProjectedPrimitives((ProjectedPrimitiveSource *)&gOvl21SceneGeometry, &eye, &target);
}
