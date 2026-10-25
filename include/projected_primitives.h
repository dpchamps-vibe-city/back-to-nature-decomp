#ifndef PROJECTED_PRIMITIVES_H
#define PROJECTED_PRIMITIVES_H

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

/* Model batches have a count at +8 and 0x1C-byte entries starting at +0xC.
 * Init locates the following culling-point block only when count >= 2.
 * Clear zeroes the two-pointer descriptor, not the batch itself. */
void InitProjectedPrimitiveSource(ProjectedPrimitiveSource *source, void *batchData);
void ClearProjectedPrimitiveSource(ProjectedPrimitiveSource *source);

/* Submit flat/Gouraud, untextured/textured triangle and quad packets into
 * the active render context and advance its packet cursor. The ordinary
 * path visits all entries; the culled path builds four planes from eye and
 * target and tests each entry's culling point before submitting it.
 * Returns the signed 16-bit primitive counter on the culled path.
 *
 * Requires initialized GTE/render state, valid format offsets and sufficient
 * packet/OT capacity. Culled sources MUST have nonzero cullPointData: its
 * original early return precedes the scratchpad-stack restore. Both paths
 * use the fixed 1KB scratchpad stack and are not interrupt-safe/reentrant
 * with another user of that stack. Matrix state is pushed/popped; clipping
 * globals and packet state are not restored. Coordinates use caller units. */
void DrawProjectedPrimitiveBatch(ProjectedPrimitiveSource *source);
s16 DrawCulledProjectedPrimitives(ProjectedPrimitiveSource *source,
                                  VECTOR *eye, VECTOR *target);

/* Install one signed ratio for all three rotation columns, enable scaling,
 * and return 1. Disable changes only the enable flag. Application is deferred
 * until drawing and skips a column unless (numerator & ~1) != 0 and its
 * denominator >= 2. Products are signed and are not overflow-checked. */
s32 SetProjectedRotationScale(s32 numerator, s32 denominator);
void DisableProjectedRotationScale(void);

#endif
