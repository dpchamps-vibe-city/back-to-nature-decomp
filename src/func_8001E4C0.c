#include "common.h"
#include "game_types.h"

/* Matched collision helpers; query types are shared through game_types.h. */
s32 func_8001E38C(void);
s32 func_8001E78C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_8001E7DC(s32 *arg0, s32 *arg1);
s32 func_8001EAE4(s32 arg0, EAE4Query *arg1);

/* GP-relative globals owned by this translation unit (tentative defs). */
BoundsStruct_8001E878 *D_8005E518;
s32 D_8005E520;
s32 D_8005E524;
s32 D_8005E528;

s32 func_8001E4C0(DriverArg_1E4C0 *arg0, BoundsStruct_8001E878 *arg1) {
    s32 i;
    s32 j;
    s32 k;
    s32 found;
    s32 count;
    s32 nslots;
    s32 off;
    s32 base[3];
    s32 vec[3];
    s16 *zp;
    s16 *zp2;
    s32 i2;
    s32 off2;
    Grid_1E4C0 *grid;
    Verts_1E4C0 *verts;
    CellGrid_1E4C0 *cell;

    D_8005E524 = 0;
    D_8005E528 = 0;
    for (k = 0; k < 8; k++) {
        (&D_80061ED8)[k] = -1;
    }
    do {
        found = -1;
    } while (0);
    verts = arg0->verts;
    grid = arg0->grid;
    count = grid->count;
    D_8005E518 = arg1;
    D_8005E520 = verts->field_0;

    do {
        do {
            do {
                do {
                    do {
                        i = 0;
                    } while (0);
                } while (0);
            } while (0);
        } while (0);
    } while (0);
    if (count > 0) {
        cell = &grid->cells[i];
        zp = (s16 *)((u8 *)verts + 8);
        off = 0;
        do {
            if (*(s32 *)((u8 *)grid + off + 0x24) != 0 &&
                func_8001E78C(zp[-2], zp[0], arg1->field_0, arg1->field_8) != 0 &&
                func_8001EAE4((s32)((u8 *)grid + 0xC), (EAE4Query *)cell) == 0) {
                found = i;
                break;
            }
            cell++;
            zp += 4;
            off += 0x1C;
        } while (++i < count);
    }

    if (found < 0) {
        return 0;
    }

    nslots = 0;
    base[0] = arg1->field_0;
    base[1] = func_8001E38C();
    base[2] = arg1->field_8;

    i2 = 0;
    if (count > 0) {
        zp2 = (s16 *)((u8 *)verts + 8);
        off2 = 0;
        do {
            if (*(s32 *)((u8 *)grid + off2 + 0x24) != 0) {
                vec[0] = zp2[-2];
                vec[1] = zp2[-1];
                vec[2] = zp2[0];
                if (func_8001E7DC(base, vec) != 0) {
                    (&D_80061ED8)[nslots] = i2;
                    nslots++;
                    if (nslots >= 8) {
                        break;
                    }
                }
            }
            off2 += 0x1C;
            zp2 += 4;
        } while (++i2 < count);
    }

    for (j = 0; j < 8; j++) {
        if (found == (&D_80061ED8)[j]) {
            (&D_80061ED8)[j] = (&D_80061ED8)[0];
            (&D_80061ED8)[0] = found;
            break;
        }
    }
    return 1;
}
