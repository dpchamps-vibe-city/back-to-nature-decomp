#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "psyq/inline_c.h"

void func_8001D348(VECTOR *arg0, VECTOR *arg1) {
    MATRIX matrices[4];
    VECTOR direction;
    VECTOR delta;
    s16 *out;
    s16 *row;
    MATRIX *matrix;
    s32 i;

    out = D_80061EA8;
    delta.vx = arg1->vx - arg0->vx;
    delta.vy = arg1->vy - arg0->vy;
    delta.vz = arg1->vz - arg0->vz;
    VectorNormal(&delta, &direction);
    D_80061EC8[0] = arg0->vx - direction.vx * 2;
    D_80061EC8[1] = arg0->vy - direction.vy * 2;
    D_80061EC8[2] = arg0->vz - direction.vz * 2;
    matrices[0] = GsIDMATRIX;
    matrices[1] = GsIDMATRIX;
    matrices[2] = GsIDMATRIX;
    matrices[3] = GsIDMATRIX;
    RotMatrixY(D_800491E0[0], &matrices[0]);
    RotMatrixY(D_800491E0[1], &matrices[1]);
    RotMatrixX(D_800491E0[2], &matrices[2]);
    RotMatrixX(D_800491E0[3], &matrices[3]);
    PushMatrix();
    for (i = 0; i < 4; i++) {
        matrix = &matrices[i];
        row = matrix->m[i / 2];
        gte_SetRotMatrix(matrix);
        gte_ldclmv((s16 *)&gCameraViewMatrix);
        gte_mvmva_core(0x4A49E012);
        gte_stclmv(matrix);
        gte_ldclmv((s16 *)&gCameraViewMatrix + 1);
        gte_mvmva_core(0x4A49E012);
        *out++ = *row++;
        gte_stclmv(&matrix->m[0][1]);
        gte_ldclmv((s16 *)&gCameraViewMatrix + 2);
        gte_mvmva_core(0x4A49E012);
        *out++ = *row++;
        gte_stclmv(&matrix->m[0][2]);
        *out++ = *row++;
    }
    PopMatrix();
}
