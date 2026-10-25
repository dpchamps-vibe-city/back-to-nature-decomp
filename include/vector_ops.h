#ifndef VECTOR_OPS_H
#define VECTOR_OPS_H

#include "common.h"
#include "game_types.h"

/* Three signed 32-bit components, in caller-defined units. Both operations
 * write x, then y, then z without a buffered aggregate copy. dest==src is
 * valid for CopyVec3; partial overlap has sequential read/write effects. */
void CopyVec3(Vec3 *dest, Vec3 *src);

/* In-place dest -= subtrahend; requires nonoverflowing signed differences.
 * No allocation, normalization or persistent state changes. */
void SubtractVec3(Vec3 *dest, Vec3 *subtrahend);

#endif
