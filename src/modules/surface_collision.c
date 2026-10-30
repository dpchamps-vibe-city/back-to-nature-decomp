/*
 * surface_collision — reconstructed translation unit.
 *
 * Query enabled surface cells and polygon records against a shared point,
 * retain the first hit, and interpolate its height.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "surface_collision.h"

s32 gSurfaceHeightTolerance;

void SetSurfaceHeightTolerance(s32 tolerance) {
    gSurfaceHeightTolerance = tolerance;
}

void EnableSurfaceCells(Grid_1E4C0 **gridAddress) {
    Grid_1E4C0 *g = *gridAddress;
    s32 i;
    s32 off;

    for (i = 0; i < g->count; i++) {
        off = i * 0x1C + 0xC;
        *(s32 *)((u8 *)g + off + 0x18) = 1;
        g = *gridAddress;
    }
}

/* The SDK member reads/writes only the three vector components. */

/* TU-owned globals (GP-relative in this translation unit). */
s32 gSurfaceTriangleHit;
BoundsStruct_8001E878 *gSurfaceQueryPoint;

s32 GetSurfaceHitHeight(void) {
    s32 result[3];
    s32 v1[3];
    s32 prod[3];
    s32 diff1[3];
    s32 diff2[3];

    if (gSurfaceTriangleHit <= 0) {
        return 0;
    }

    v1[0] = gSurfaceHitVertices[1]->x;
    v1[1] = gSurfaceHitVertices[1]->y;
    v1[2] = gSurfaceHitVertices[1]->z;

    diff1[0] = v1[0] - gSurfaceHitVertices[0]->x;
    diff1[1] = v1[1] - gSurfaceHitVertices[0]->y;
    diff1[2] = v1[2] - gSurfaceHitVertices[0]->z;

    diff2[0] = gSurfaceHitVertices[2]->x - v1[0];
    diff2[1] = gSurfaceHitVertices[2]->y - v1[1];
    diff2[2] = gSurfaceHitVertices[2]->z - v1[2];

    OuterProduct0((VECTOR *)diff1, (VECTOR *)diff2, (VECTOR *)result);

    prod[0] = (gSurfaceQueryPoint->field_0 - v1[0]) * result[0];
    prod[2] = (gSurfaceQueryPoint->field_8 - v1[2]) * result[2];
    prod[1] = -(prod[0] + prod[2]);

    if (result[1] == 0) {
        return gSurfaceQueryPoint->field_4;
    }

    return (prod[1] / result[1]) + v1[1];
}

/* Matched collision helpers; query types are shared through game_types.h. */
s32 GetSurfaceHitHeight(void);
s32 IsSurfacePointNear2D(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 IsSurfacePointNear3D(s32 *arg0, s32 *arg1);
s32 ScanSurfacePolygons(s32 arg0, EAE4Query *arg1);

/* GP-relative globals owned by this translation unit (tentative defs). */
s32 gSurfaceProximityExtent;
s32 gSurfaceHitPolygonAddress;

s32 QuerySurfaceCells(DriverArg_1E4C0 *arg0, BoundsStruct_8001E878 *arg1) {
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

    gSurfaceHitPolygonAddress = 0;
    gSurfaceTriangleHit = 0;
    for (k = 0; k < 8; k++) {
        D_80061ED8[k] = -1;
    }
    do {
        found = -1;
    } while (0);
    verts = arg0->verts;
    grid = arg0->grid;
    count = grid->count;
    gSurfaceQueryPoint = arg1;
    gSurfaceProximityExtent = verts->field_0;

    /* Retain the original loop-note nesting: collapsing these empty scopes
     * changes this compiler's driver code by one instruction. */
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
                IsSurfacePointNear2D(zp[-2], zp[0], arg1->field_0, arg1->field_8) != 0 &&
                ScanSurfacePolygons((s32)((u8 *)grid + 0xC), (EAE4Query *)cell) == 0) {
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
    base[1] = GetSurfaceHitHeight();
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
                if (IsSurfacePointNear3D(base, vec) != 0) {
                    D_80061ED8[nslots] = i2;
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
        if (found == D_80061ED8[j]) {
            D_80061ED8[j] = D_80061ED8[0];
            D_80061ED8[0] = found;
            break;
        }
    }
    return 1;
}

/* Retry the eight cell indices QuerySurfaceCells retained in D_80061ED8 (-1
 * marks an empty slot): reset the hit address, install the shared query
 * pointer, then scan each listed cell's polygon run in slot order. A negative
 * index is skipped; the first cell whose scan finds a polygon returns 1. All
 * eight slots exhausted returns 0 and leaves no hit behind. */
s32 func_8001E6FC(Grid_1E4C0 **gridAddress, BoundsStruct_8001E878 *query) {
    Grid_1E4C0 *grid;
    s32 i;
    s32 index;

    grid = *gridAddress;
    gSurfaceHitPolygonAddress = 0;
    gSurfaceQueryPoint = query;
    for (i = 0; i < 8; i++) {
        index = D_80061ED8[i];
        if (index >= 0) {
            if (ScanSurfacePolygons((s32)((u8 *)grid + 0xC),
                                    (EAE4Query *)((u8 *)grid + (index * 0x1C + 0xC))) == 0) {
                return 1;
            }
        }
    }
    return 0;
}

/* Proximity test on a 2D pair: returns 1 when both component deltas fall
 * strictly inside +/-bound, where bound = (D_8005E520 >> 1) + 600. Any
 * component reaching the bound returns 0. func_8001E7DC is the 3-component
 * form of the same test over the same tolerance global.
 *
 * The subtractions assign back into the parameters. That is load-bearing:
 * a separate delta variable is a fresh pseudo whose allocno inherits the
 * argument registers as hard-register preferences, so it lands on $a3 and
 * forces a second argument copy. Reusing the parameter keeps the delta on
 * the incoming web, which is what produces the entry `move v1,a1` and the
 * in-place `subu v1,v1,a3`. */
s32 IsSurfacePointNear2D(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 bound;
    s32 lower;

    bound = (gSurfaceProximityExtent >> 1) + 0x258;
    lower = -bound;

    arg0 = arg0 - arg2;
    if (arg0 <= lower || arg0 >= bound) {
        return 0;
    }

    arg1 = arg1 - arg3;
    if (arg1 <= lower || arg1 >= bound) {
        return 0;
    }

    return 1;
}

s32 IsSurfacePointNear3D(s32 *arg0, s32 *arg1) {
    s32 bound;
    s32 lower;
    s32 delta;

    delta = *arg0++ - *arg1++;
    bound = (gSurfaceProximityExtent >> 1) + 0x258;
    lower = -bound;
    if (delta < lower) {
        goto fail;
    }
    if (!(bound < delta)) {
        goto check_y;
    }
fail:
    return 0;
check_y:
    delta = *arg0++ - *arg1++;
    if (delta < lower) {
        goto fail;
    }
    if (bound < delta) {
        goto fail;
    }
    delta = *arg0 - *arg1;
    if (delta < lower) {
        goto fail;
    }
    if (bound < delta) {
        goto fail;
    }
    return 1;
}

/* GP-relative globals owned by this translation unit (tentative defs). */
s32 gSurfacePolygonRejectMask;
s32 gSurfaceVertexBase;
s32 gSurfacePolygonsRemaining;
s32 gSurfacePolygonCursor;
s32 gSurfaceActiveRejectMask;
s32 gSurfaceCrossAB;
s32 gSurfaceCrossBC;
s32 gSurfaceCrossCA;
s32 gSurfaceQueryX;
s32 gSurfaceQueryZ;
s32 gSurfaceHeightDelta;

/* Eight ordered format runs, each with its own vertex-index offsets and
 * record byte stride. A format not belonging to the current or a later run
 * terminates the scan; the asset must retain this order. The header mask
 * ignores bit 0x02000000 and the low halfword, not the reject-mask bits. */
#define EAE4_TYPE_0 0x21010000
#define EAE4_TYPE_1 0x31010000
#define EAE4_TYPE_2 0x25010000
#define EAE4_TYPE_3 0x35010000
#define EAE4_TYPE_4 0x29010000
#define EAE4_TYPE_5 0x39010000
#define EAE4_TYPE_6 0x2D010000
#define EAE4_TYPE_7 0x3D010000
#define EAE4_TYPE_MASK 0xFDFF0000

s32 ScanSurfacePolygons(s32 arg0, EAE4Query *arg1) {
    /* Real GNU nesting supplies the static-chain save/forward sequence in
     * both helpers. Symbol labels preserve their original binary identities;
     * no hard-register capture is needed. */
    auto s32 testTriangle(SurfaceVertexView *p0, SurfaceVertexView *p1, SurfaceVertexView *p2) __asm__("TestSurfaceTriangle");
    auto s32 testQuad(s32 i0, s32 i1, s32 i2, s32 i3) __asm__("TestSurfaceQuad");
    s32 base0, type0;
    s32 base1, type1;
    s32 base2, type2;
    s32 base3, type3;
    s32 base4, type4;
    s32 base5, type5;
    s32 base6, type6;
    s32 base7, type7;
    s32 count = arg1->field_14;
    s32 offset0 = arg1->field_0;
    s32 offset10 = arg1->field_10;

    /* Accept either winding, including points on triangle edges. The height
     * gate uses average vertex Y, not the interpolated plane height. */
    s32 testTriangle(SurfaceVertexView *p0, SurfaceVertexView *p1, SurfaceVertexView *p2) {
        s32 averageY;
        s32 heightDelta;
        s32 queryX;
        s32 queryZ;
        s32 crossAB;
        s32 crossBC;
        s32 crossCA;

        averageY = (p0->y + p1->y + p2->y) / 3;
        gSurfaceHeightDelta = averageY;
        heightDelta = gSurfaceQueryPoint->field_4 - averageY;
        gSurfaceHeightDelta = heightDelta;
        if (heightDelta > gSurfaceHeightTolerance) {
            return 1;
        }
        if (heightDelta < -gSurfaceHeightTolerance) {
            return 1;
        }
        queryX = gSurfaceQueryPoint->field_0;
        queryZ = gSurfaceQueryPoint->field_8;
        gSurfaceQueryX = queryX;
        gSurfaceQueryZ = queryZ;
        crossAB = (p1->x - p0->x) * (queryZ - p0->z)
                - (p1->z - p0->z) * (queryX - p0->x);
        gSurfaceCrossAB = crossAB;
        crossBC = (p2->x - p1->x) * (queryZ - p1->z)
                - (p2->z - p1->z) * (queryX - p1->x);
        gSurfaceCrossBC = crossBC;
        crossCA = (p0->x - p2->x) * (queryZ - p2->z)
                - (p0->z - p2->z) * (queryX - p2->x);
        gSurfaceCrossCA = crossCA;
        if ((gSurfaceCrossAB <= 0 && gSurfaceCrossBC <= 0 && gSurfaceCrossCA <= 0)
         || (gSurfaceCrossAB >= 0 && gSurfaceCrossBC >= 0 && gSurfaceCrossCA >= 0)) {
            goto inside;
        }
        return 1;
    inside:
        gSurfaceTriangleHit = 1;
        ((s32 *)&gSurfaceHitVertices)[0] = (s32)p0;
        ((s32 *)&gSurfaceHitVertices)[1] = (s32)p1;
        ((s32 *)&gSurfaceHitVertices)[2] = (s32)p2;
        return 0;
    }

    /* Repeated corner indices reduce a quad to one triangle. */
    s32 testQuad(s32 i0, s32 i1, s32 i2, s32 i3) {
        s32 offset2;
        s32 offset1;

        if ((i0 == i1) || (i0 == i2)) {
            return testTriangle((SurfaceVertexView *)(gSurfaceVertexBase + i2 * 8),
                                (SurfaceVertexView *)(gSurfaceVertexBase + i1 * 8),
                                (SurfaceVertexView *)(gSurfaceVertexBase + i3 * 8));
        }
        if ((i3 == i1) || (i3 == i2)) {
            return testTriangle((SurfaceVertexView *)(gSurfaceVertexBase + i0 * 8),
                                (SurfaceVertexView *)(gSurfaceVertexBase + i1 * 8),
                                (SurfaceVertexView *)(gSurfaceVertexBase + i2 * 8));
        }
        offset1 = i1 * 8;
        offset2 = i2 * 8;
        if (testTriangle((SurfaceVertexView *)(gSurfaceVertexBase + i0 * 8),
                         (SurfaceVertexView *)(gSurfaceVertexBase + offset1),
                         (SurfaceVertexView *)(gSurfaceVertexBase + offset2)) == 0) {
            return 0;
        }
        return testTriangle((SurfaceVertexView *)(gSurfaceVertexBase + i3 * 8),
                            (SurfaceVertexView *)(gSurfaceVertexBase + offset2),
                            (SurfaceVertexView *)(gSurfaceVertexBase + offset1));
    }

    gSurfaceHitPolygonAddress = 0;
    gSurfacePolygonsRemaining = count;
    gSurfaceVertexBase = arg0 + offset0;
    gSurfacePolygonCursor = arg0 + offset10;
    gSurfaceActiveRejectMask = gSurfacePolygonRejectMask;

loop0:
    if (gSurfacePolygonsRemaining == 0) goto ret1;
    base0 = gSurfacePolygonCursor;
    type0 = *(s32 *)base0;
    if ((type0 & EAE4_TYPE_MASK) != EAE4_TYPE_0) goto loop1;
    if ((type0 & gSurfaceActiveRejectMask) == 0) {
        if (testTriangle((SurfaceVertexView *)(gSurfaceVertexBase + ((u16 *)base0)[4] * 8), (SurfaceVertexView *)(gSurfaceVertexBase + ((u16 *)base0)[5] * 8), (SurfaceVertexView *)(gSurfaceVertexBase + ((u16 *)base0)[6] * 8)) == 0) {
            goto ret0;
        }
    }
    gSurfacePolygonCursor += 0x10;
    gSurfacePolygonsRemaining--;
    goto loop0;

loop1:
    if (gSurfacePolygonsRemaining == 0) goto ret1;
    base1 = gSurfacePolygonCursor;
    type1 = *(s32 *)base1;
    if ((type1 & EAE4_TYPE_MASK) != EAE4_TYPE_1) goto loop2;
    if ((type1 & gSurfaceActiveRejectMask) == 0) {
        if (testTriangle((SurfaceVertexView *)(gSurfaceVertexBase + ((u16 *)base1)[8] * 8), (SurfaceVertexView *)(gSurfaceVertexBase + ((u16 *)base1)[9] * 8), (SurfaceVertexView *)(gSurfaceVertexBase + ((u16 *)base1)[10] * 8)) == 0) {
            goto ret0;
        }
    }
    gSurfacePolygonCursor += 0x18;
    gSurfacePolygonsRemaining--;
    goto loop1;

loop2:
    if (gSurfacePolygonsRemaining == 0) goto ret1;
    base2 = gSurfacePolygonCursor;
    type2 = *(s32 *)base2;
    if ((type2 & EAE4_TYPE_MASK) != EAE4_TYPE_2) goto loop3;
    if ((type2 & gSurfaceActiveRejectMask) == 0) {
        if (testTriangle((SurfaceVertexView *)(gSurfaceVertexBase + ((u16 *)base2)[10] * 8), (SurfaceVertexView *)(gSurfaceVertexBase + ((u16 *)base2)[11] * 8), (SurfaceVertexView *)(gSurfaceVertexBase + ((u16 *)base2)[12] * 8)) == 0) {
            goto ret0;
        }
    }
    gSurfacePolygonCursor += 0x1C;
    gSurfacePolygonsRemaining--;
    goto loop2;

loop3:
    if (gSurfacePolygonsRemaining == 0) goto ret1;
    base3 = gSurfacePolygonCursor;
    type3 = *(s32 *)base3;
    if ((type3 & EAE4_TYPE_MASK) != EAE4_TYPE_3) goto loop4;
    if ((type3 & gSurfaceActiveRejectMask) == 0) {
        if (testTriangle((SurfaceVertexView *)(gSurfaceVertexBase + ((u16 *)base3)[14] * 8), (SurfaceVertexView *)(gSurfaceVertexBase + ((u16 *)base3)[15] * 8), (SurfaceVertexView *)(gSurfaceVertexBase + ((u16 *)base3)[16] * 8)) == 0) {
            goto ret0;
        }
    }
    gSurfacePolygonCursor += 0x24;
    gSurfacePolygonsRemaining--;
    goto loop3;

loop4:
    if (gSurfacePolygonsRemaining == 0) goto ret1;
    base4 = gSurfacePolygonCursor;
    type4 = *(s32 *)base4;
    if ((type4 & EAE4_TYPE_MASK) != EAE4_TYPE_4) goto loop5;
    if ((type4 & gSurfaceActiveRejectMask) == 0) {
        if (testQuad(((u16 *)base4)[4], ((u16 *)base4)[5], ((u16 *)base4)[6], ((u16 *)base4)[7]) == 0) {
            goto ret0;
        }
    }
    gSurfacePolygonCursor += 0x10;
    gSurfacePolygonsRemaining--;
    goto loop4;

loop5:
    if (gSurfacePolygonsRemaining == 0) goto ret1;
    base5 = gSurfacePolygonCursor;
    type5 = *(s32 *)base5;
    if ((type5 & EAE4_TYPE_MASK) != EAE4_TYPE_5) goto loop6;
    if ((type5 & gSurfaceActiveRejectMask) == 0) {
        if (testQuad(((u16 *)base5)[10], ((u16 *)base5)[11], ((u16 *)base5)[12], ((u16 *)base5)[13]) == 0) {
            goto ret0;
        }
    }
    gSurfacePolygonCursor += 0x1C;
    gSurfacePolygonsRemaining--;
    goto loop5;

loop6:
    if (gSurfacePolygonsRemaining == 0) goto ret1;
    base6 = gSurfacePolygonCursor;
    type6 = *(s32 *)base6;
    if ((type6 & EAE4_TYPE_MASK) != EAE4_TYPE_6) goto loop7;
    if ((type6 & gSurfaceActiveRejectMask) == 0) {
        if (testQuad(((u16 *)base6)[12], ((u16 *)base6)[13], ((u16 *)base6)[14], ((u16 *)base6)[15]) == 0) {
            goto ret0;
        }
    }
    gSurfacePolygonCursor += 0x20;
    gSurfacePolygonsRemaining--;
    goto loop6;

loop7:
    if (gSurfacePolygonsRemaining == 0) goto ret1;
    base7 = gSurfacePolygonCursor;
    type7 = *(s32 *)base7;
    if ((type7 & EAE4_TYPE_MASK) != EAE4_TYPE_7) goto ret1;
    if ((type7 & gSurfaceActiveRejectMask) == 0) {
        if (testQuad(((u16 *)base7)[18], ((u16 *)base7)[19], ((u16 *)base7)[20], ((u16 *)base7)[21]) == 0) {
            goto ret0;
        }
    }
    gSurfacePolygonCursor += 0x2C;
    gSurfacePolygonsRemaining--;
    goto loop7;

ret1:
    return 1;

ret0:
    gSurfaceHitPolygonAddress = gSurfacePolygonCursor;
    return 0;
}

void SetSurfacePolygonRejectMask(s32 mask) {
    gSurfacePolygonRejectMask = mask;
}

s32 GetSurfaceHitPolygonAddress(void) {
    return gSurfaceHitPolygonAddress;
}
