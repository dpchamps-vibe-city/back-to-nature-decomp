/*
 * GTE scene state, original exe:0x8001B530-0x8001BFA8. Camera construction,
 * projection defaults, matrix snapshot/composition, fog and lighting settings.
 * The GP-relative RGB/fog settings join the apply routine to the tail setters;
 * the camera/matrix data and internal calls join the remaining members.
 * Six pre-existing dead helper stubs remain in their original positions.
 * See notes/file-groupings.md for the membership and boundary evidence.
 */
#include "common.h"
#include "gte_scene_state.h"
#include "gte_transform.h"
#include "psyq/memory.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "psyq/inline_c.h"
#include "psyq/inline_c_gas.h"

/* Tentative definitions preserve the original small-data ownership. */
s32 gSceneFogNear;
u8 gSceneBackRed;
u8 gSceneBackGreen;
u8 gSceneBackBlue;
u8 gSceneFarRed;
u8 gSceneFarGreen;
u8 gSceneFarBlue;

void BuildAndLoadCameraView(void);

void InitGteSceneState(void) {
    SetGeomOffset(0xA0, 0x78);
    SetGeomScreen(0x3E8);
    SetCameraEyeAndRoll(0, 0, 0x1000, 0x800);
    SetCameraTarget(0, 0, -0x1000);
    memset(&gSceneLightMatrix, 0, 0x20);
    memset(&gSceneColorMatrix, 0, 0x20);
    memset(&gSceneDefaultMatrix, 0, 0x20);
    gSceneDefaultMatrix.field_10 = 0x1000;
    gSceneDefaultMatrix.field_8 = 0x1000;
    gSceneDefaultMatrix.field_0 = 0x1000;
}

void ApplyGteSceneState(void) {
    BuildAndLoadCameraView();
    SetFogNear(gSceneFogNear, 0x3E8);
    gte_SetBackColor(gSceneBackRed, gSceneBackGreen, gSceneBackBlue);
    SetFarColor(gSceneFarRed, gSceneFarGreen, gSceneFarBlue);
    gte_SetColorMatrix(&gSceneColorMatrix);
    gte_SetLightMatrix(&gSceneLightMatrix);
}

void BuildAndLoadCameraView(void) {
    VECTOR delta;
    VECTOR forward;
    VECTOR right;
    MATRIX roll;
    s32 i;
    s16 *row;
    s32 *f;
    s32 *r;
    s32 *rightBase;
    s32 *forwardBase;
    VECTOR *cross;
    s16 *rightNormal;
    s16 *up;
    s16 *check;
    s16 *side;
    s32 x;
    s32 y;

    if (gCameraViewParameters.matrixOverrideAddress == 0) {
        delta.vx = gCameraViewParameters.targetX - gCameraViewParameters.eyeX;
        delta.vy = gCameraViewParameters.targetY - gCameraViewParameters.eyeY;
        delta.vz = gCameraViewParameters.targetZ - gCameraViewParameters.eyeZ;
        VectorNormalS(&delta, (SVECTOR *)(gCameraViewMatrix + 6));
        check = gCameraViewMatrix;
        delta.vy = 0;
        /* A vertical view needs a fallback right vector instead of the
         * horizontal perpendicular of the forward vector. */
        if (check[6] == 0 && check[8] == 0) {
            delta.vx = 0x1000;
            delta.vz = 0;
        } else {
            side = gCameraViewMatrix;
            delta.vx = side[8];
            delta.vz = -side[6];
        }
        rightNormal = gCameraViewMatrix;
        VectorNormalS(&delta, (SVECTOR *)rightNormal);
        row = rightNormal;
        cross = &delta;
        rightBase = (s32 *)&right;
        r = rightBase;
        forwardBase = (s32 *)&forward;
        f = forwardBase;
        for (i = 0; i < 3; i++) {
            x = row[6];
            y = *row++;
            *f = x;
            *r++ = y;
            f++;
        }
        gte_ldopv1(forwardBase);
        gte_ldopv2(rightBase);
        gte_mvmva_core(0x4B78000C);
        gte_stlvnl(cross);
        up = gCameraViewMatrix + 3;
        VectorNormalS(cross, (SVECTOR *)up);
        if (gCameraViewParameters.rollAngle != 0) {
            roll = GsIDMATRIX;
            RotMatrixZ(gCameraViewParameters.rollAngle, &roll);
            gte_SetRotMatrix(&roll);
            /* Keep the three separately materialized column addresses; the
             * SDK GTE macros consume them as register operands. */
            x = (s32)(up - 3);
            gte_ldclmv((s16 *)x);
            gte_mvmva_core(0x4A49E012);
            gte_stclmv((s16 *)x);
            y = (s32)(up - 2);
            gte_ldclmv((s16 *)y);
            gte_mvmva_core(0x4A49E012);
            gte_stclmv((s16 *)y);
            x = (s32)(up - 1);
            gte_ldclmv((s16 *)x);
            gte_mvmva_core(0x4A49E012);
            gte_stclmv((s16 *)x);
        }
        delta.vx = -gCameraViewParameters.eyeX;
        delta.vy = -gCameraViewParameters.eyeY;
        delta.vz = -gCameraViewParameters.eyeZ;
        ApplyMatrixLV((MATRIX *)(up - 3), cross, (VECTOR *)(up + 7));
    }
    gte_SetRotMatrix(gCameraViewMatrix);
    gte_SetTransMatrix(gCameraViewMatrix);
}

void SetCameraEyeAndRoll(s32 x, s32 y, s32 z, s32 rollAngle) {
    gCameraViewParameters.eyeX = x;
    gCameraViewParameters.eyeY = y;
    gCameraViewParameters.eyeZ = z;
    gCameraViewParameters.rollAngle = rollAngle;
    gCameraViewParameters.matrixOverrideAddress = 0;
}

INCLUDE_ASM("build/asm/nonmatchings/modules/gte_scene_state", GetCameraEyePosition);

void SetCameraTarget(s32 x, s32 y, s32 z) {
    gCameraViewParameters.targetX = x;
    gCameraViewParameters.targetY = y;
    gCameraViewParameters.targetZ = z;
    gCameraViewParameters.matrixOverrideAddress = 0;
}

/* Dead matrix-copy APIs establish the override-address meaning of the final
 * camera word; they are retained, not replaced with new implementations. */
INCLUDE_ASM("build/asm/nonmatchings/modules/gte_scene_state", SetCameraViewMatrix);
INCLUDE_ASM("build/asm/nonmatchings/modules/gte_scene_state", GetCameraViewMatrix);
INCLUDE_ASM("build/asm/nonmatchings/modules/gte_scene_state", func_8001BB28);

void CaptureGteTransform(void) {
    gte_ReadRotMatrix(&gCapturedGteTransform);
}

void ComposeCapturedGteTransform(MATRIX *local) {
    gte_SetRotMatrix(&gCapturedGteTransform);
    gte_ldclmv(&local->m[0][0]);
    gte_rtir();
    gte_stclmv(&gCapturedGteTransform.m[0][0]);
    gte_ldclmv(&local->m[0][1]);
    gte_rtir();
    gte_stclmv(&gCapturedGteTransform.m[0][1]);
    gte_ldclmv(&local->m[0][2]);
    gte_rtir();
    gte_stclmv(&gCapturedGteTransform.m[0][2]);
    gte_SetTransMatrix(&gCapturedGteTransform);
    gte_ldlv0(&local->t[0]);
    gte_rtv0tr();
    gte_stlvl(&gCapturedGteTransform.t[0]);
    gte_SetRotMatrix(&gCapturedGteTransform);
    gte_SetTransMatrix(&gCapturedGteTransform);
}

INCLUDE_ASM("build/asm/nonmatchings/modules/gte_scene_state", SetDirectionalLightVector);
INCLUDE_ASM("build/asm/nonmatchings/modules/gte_scene_state", func_8001BE18);

void SetSceneBackColor(s32 red, s32 green, s32 blue) {
    gSceneBackRed = red;
    gSceneBackGreen = green;
    gSceneBackBlue = blue;
}

void SetSceneFarColor(s32 red, s32 green, s32 blue) {
    gSceneFarRed = red;
    gSceneFarGreen = green;
    gSceneFarBlue = blue;
}

void SetSceneFogNear(s32 distance) {
    gSceneFogNear = distance;
}
