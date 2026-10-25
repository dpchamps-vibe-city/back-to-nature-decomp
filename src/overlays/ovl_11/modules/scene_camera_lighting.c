/*
 * Overlay-11 camera orbit, scene geometry drawing and fixed lighting setup.
 *
 * The five camera functions share one narrow work-area state block. The
 * intervening lighting/projection setup is used with the reset functions by
 * scene startup and owns the three private flat-light records. Membership
 * evidence and boundaries are recorded in notes/file-groupings.md; interface
 * units, ordering and state effects are in ovl11_scene_camera_lighting.h.
 */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "gte_scene_state.h"
#include "projected_primitives.h"
#include "ovl11_scene_camera_lighting.h"

/* Offsets from D_8007AFF0, not independent globals. Word resets and low-
 * halfword consumers retain their original widths. Two reset-only words
 * at +2539C/+253A4 have no established role here. */
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

void Ovl11UpdateSceneCameraOffset(void) {
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

void Ovl11ApplySceneCamera(void) {
    char *camera = (char *)&D_8007AFF0;

    SetCameraEyeAndRoll(*(s16 *)(camera + CAMERA_OFFSET_X) + *(s16 *)(camera + CAMERA_TARGET_X),
                       *(s16 *)(camera + CAMERA_EYE_Y),
                       *(s16 *)(camera + CAMERA_OFFSET_Z) + *(s16 *)(camera + CAMERA_TARGET_Z),
                       0);
    SetCameraTarget(*(s16 *)(camera + CAMERA_TARGET_X),
                    *(s16 *)(camera + CAMERA_TARGET_Y),
                    *(s16 *)(camera + CAMERA_TARGET_Z));
}

void Ovl11ResetSceneCameraOrbit(void) {
    char *camera = (char *)&D_8007AFF0;

    *(s32 *)(camera + CAMERA_DISTANCE) = -0x2328;
    *(s32 *)(camera + CAMERA_HEIGHT) = -0x1194;
    *(s32 *)(camera + 0x2539C) = 0;
    *(s32 *)(camera + CAMERA_YAW) = -0x200;
    *(s32 *)(camera + 0x253A4) = 0;
}

void Ovl11InitSceneLighting(void) {
    gOvl11SceneLights[0].vx = 0x64;
    gOvl11SceneLights[0].vy = 0x64;
    gOvl11SceneLights[0].vz = 0x64;
    gOvl11SceneLights[0].r = 0xD0;
    gOvl11SceneLights[0].g = 0xD0;
    gOvl11SceneLights[0].b = 0xD0;
    GsSetFlatLight(0, (GsF_LIGHT *)&gOvl11SceneLights[0]);
    gOvl11SceneLights[1].vx = 0x64;
    gOvl11SceneLights[1].vy = 0x64;
    gOvl11SceneLights[1].vz = 0x64;
    gOvl11SceneLights[1].r = 0;
    gOvl11SceneLights[1].g = 0;
    gOvl11SceneLights[1].b = 0;
    GsSetFlatLight(1, (GsF_LIGHT *)&gOvl11SceneLights[1]);
    gOvl11SceneLights[2].vx = 0x64;
    gOvl11SceneLights[2].vy = 0x64;
    gOvl11SceneLights[2].vz = 0x64;
    gOvl11SceneLights[2].r = 0;
    gOvl11SceneLights[2].g = 0;
    gOvl11SceneLights[2].b = 0;
    GsSetFlatLight(2, (GsF_LIGHT *)&gOvl11SceneLights[2]);
    GsSetAmbient(0x800, 0x800, 0x800);
    GsSetLightMode(0);
}

void Ovl11SetSceneProjection(void) {
    GsSetProjection(0x3E8);
    SetGeomScreen(0x3E8);
}

void Ovl11DrawSceneGeometry(void) {
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
    DrawCulledProjectedPrimitives((ProjectedPrimitiveSource *)&gOvl11SceneGeometry, &eye, &target);
}

void Ovl11ClearSceneCameraVectors(void) {
    char *camera = (char *)&D_8007AFF0;

    *(s16 *)(camera + CAMERA_OFFSET_X) = 0;
    *(s16 *)(camera + CAMERA_EYE_Y) = 0;
    *(s16 *)(camera + CAMERA_OFFSET_Z) = 0;
    *(s16 *)(camera + CAMERA_TARGET_X) = 0;
    *(s16 *)(camera + CAMERA_TARGET_Y) = 0;
    *(s16 *)(camera + CAMERA_TARGET_Z) = 0;
}
