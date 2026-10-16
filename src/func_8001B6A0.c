#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "psyq/inline_c.h"

void func_8001B6A0(void) {
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
    s16 *column;
    s16 *column2;
    s16 *up;
    s16 *look;
    s16 *check;
    s16 *side;
    s32 x;
    s32 y;

    if (D_80061DE8.field_1C == 0) {
        delta.vx = D_80061DE8.field_0C - D_80061DE8.field_00;
        delta.vy = D_80061DE8.field_10 - D_80061DE8.field_04;
        delta.vz = D_80061DE8.field_14 - D_80061DE8.field_08;
        VectorNormalS(&delta, (SVECTOR *)(D_80061E08 + 6));
        check = D_80061E08;
        delta.vy = 0;
        if (check[6] == 0 && check[8] == 0) {
            delta.vx = 0x1000;
            delta.vz = 0;
        } else {
            side = D_80061E08;
            delta.vx = side[8];
            delta.vz = -side[6];
        }
        rightNormal = D_80061E08;
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
        up = D_80061E08 + 3;
        VectorNormalS(cross, (SVECTOR *)up);
        if (D_80061DE8.field_18 != 0) {
            roll = GsIDMATRIX;
            RotMatrixZ(D_80061DE8.field_18, &roll);
            gte_SetRotMatrix(&roll);
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
        delta.vx = -D_80061DE8.field_00;
        delta.vy = -D_80061DE8.field_04;
        delta.vz = -D_80061DE8.field_08;
        ApplyMatrixLV((MATRIX *)(up - 3), cross, (VECTOR *)(up + 7));
    }
    gte_SetRotMatrix(D_80061E08);
    gte_SetTransMatrix(D_80061E08);
}
