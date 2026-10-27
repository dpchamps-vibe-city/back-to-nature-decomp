#ifndef SURFACE_COLLISION_H
#define SURFACE_COLLISION_H

#include "common.h"
#include "game_types.h"

/* Shared, non-reentrant surface query. All coordinates/tolerances use the
 * geometry's integer world units, not the callers' 12-bit fixed-point units.
 * Geometry must remain resident through all hit getters. No bounds validation
 * is performed on cell offsets, polygon runs or vertex indices. */

/* Enable every 0x1C-byte cell through the grid pointer at *gridAddress.
 * Does not initialize the vertex data or either query tolerance. */
void EnableSurfaceCells(Grid_1E4C0 **gridAddress);

/* Accept a triangle only when abs(queryY - averageVertexY) <= tolerance.
 * The value is stored verbatim; callers normally use nonnegative tolerances. */
void SetSurfaceHeightTolerance(s32 tolerance);

/* Skip a polygon when any header bit overlaps mask. Snapshot at scan entry. */
void SetSurfacePolygonRejectMask(s32 mask);

/* Return 1 on the first accepted polygon, 0 on failure. Installs position as
 * the shared query pointer, resets hit state and builds at most eight nearby
 * cell indices, moving the hit cell to slot zero. The driver uses strict X/Z
 * proximity; the subsequent nearby-cell test includes its boundary. */
s32 QuerySurfaceCells(DriverArg_1E4C0 *geometry, BoundsStruct_8001E878 *position);

/* Interpolate the retained triangle plane at query X/Z using integer division.
 * No triangle: 0. Zero Y normal: query Y. Does not perform a fresh query. */
s32 GetSurfaceHitHeight(void);

/* Raw address-as-s32 ABI, 0 without a hit. The first word is the polygon header,
 * not an independent flags allocation. May be invalidated by the next query. */
s32 GetSurfaceHitPolygonAddress(void);

#endif
