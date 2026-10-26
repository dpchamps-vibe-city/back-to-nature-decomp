#ifndef OVL11_MODE6_GRID_H
#define OVL11_MODE6_GRID_H

#include "common.h"

/* Build/resume the 7x7 scene-surface pointer grid (physical stride 45).
 * A zero low halfword of resume resets the scan row and position; otherwise
 * continue from the saved row. Return 1, including when already complete.
 * Cell centers: X=-1420+400*column, Z=1020-400*row. Each sample runs the
 * shared surface query with mode 0xC000; install its last polygon only when
 * bit 3 is set. Other cells are NOT cleared. The query's success return is
 * ignored: callers must initialize valid scene geometry/last-hit state.
 * Modifies scan scratch and shared polygon-query state; not reentrant. */
s32 Ovl11BuildMode6SurfaceGrid(s32 resume);

#endif
