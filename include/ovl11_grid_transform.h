#ifndef OVL11_GRID_TRANSFORM_H
#define OVL11_GRID_TRANSFORM_H

#include "common.h"

/* Original ovl_11:800D6488. Remap 13x13 byte cells: mode 0 transposes,
 * 1 mirrors columns, 2 rotates 90 degrees counterclockwise, 3 copies.
 * Other modes leave dst untouched. No allocation or persistent state change.
 * Buffers must contain all 169 bytes; use nonoverlapping buffers for these
 * geometric results. Overlap is processed in traversal order, not snapshotted.
 * The mode is an unsigned byte; callers passing a word must narrow it. */
void Ovl11TransformGridCells(u8 mode, u8 dst[][13], u8 src[][13]);

#endif
