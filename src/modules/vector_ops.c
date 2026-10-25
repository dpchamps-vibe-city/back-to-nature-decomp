/* Three-word vector operations. The distance constructor calls this unit's
 * copy/subtract pair; related add/compare leaves remain in original order.
 * Three pre-existing unused assembly placeholders are retained, not newly
 * introduced implementations. Membership/boundaries: notes/file-groupings.md. */

#include "vector_ops.h"
#include "include_asm.h"

void CopyVec3(Vec3 *dest, Vec3 *src) {
    dest->x = src->x;
    dest->y = src->y;
    dest->z = src->z;
}

/* Unused in-place three-word addition. */
INCLUDE_ASM("build/asm/nonmatchings/modules/vector_ops", func_8001F35C);

void SubtractVec3(Vec3 *dest, Vec3 *subtrahend) {
    dest->x = dest->x - subtrahend->x;
    dest->y = dest->y - subtrahend->y;
    dest->z = dest->z - subtrahend->z;
}

/* Unused three-component inequality test. */
INCLUDE_ASM("build/asm/nonmatchings/modules/vector_ops", func_8001F3DC);

/* Unused constructor: copy input, subtract origin, measure distance and
 * optionally write origin + delta*requestedDistance/measuredDistance.
 * Its existing placeholder is not a new public interface. */
INCLUDE_ASM("build/asm/nonmatchings/modules/vector_ops", func_8001F42C);
