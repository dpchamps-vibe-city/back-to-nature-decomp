/* Three-word vector operations. The distance constructor calls this unit's
 * copy/subtract pair; its add and inequality companions are unused leaves.
 * Membership/boundaries: notes/file-groupings.md. */

#include "vector_ops.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

void CopyVec3(Vec3 *dest, Vec3 *src) {
    dest->x = src->x;
    dest->y = src->y;
    dest->z = src->z;
}

/* Unused in-place three-word addition. */
void func_8001F35C(Vec3 *dest, Vec3 *addend) {
    dest->x = dest->x + addend->x;
    dest->y = dest->y + addend->y;
    dest->z = dest->z + addend->z;
}

void SubtractVec3(Vec3 *dest, Vec3 *subtrahend) {
    dest->x = dest->x - subtrahend->x;
    dest->y = dest->y - subtrahend->y;
    dest->z = dest->z - subtrahend->z;
}

/* Unused three-component inequality test: nonzero when any word differs. */
s32 func_8001F3DC(Vec3 *first, Vec3 *second) {
    if (first->x != second->x) {
        return 1;
    }
    if (first->y != second->y) {
        return 1;
    }
    return first->z != second->z;
}

/* Unused constructor: measure the offset from origin to source, then write
 * origin + offset*requestedDistance/measuredDistance to dest. A zero-length
 * offset copies the origin instead, and a null dest only measures. Returns 1
 * while the measured offset is shorter than the requested distance. */
s32 func_8001F42C(Vec3 *source, s32 distance, Vec3 *origin, Vec3 *dest) {
    Vec3 offset;
    s32 length;
    s32 result;

    result = 1;
    CopyVec3(&offset, source);
    SubtractVec3(&offset, origin);
    length = SquareRoot0(offset.x * offset.x + offset.y * offset.y + offset.z * offset.z);
    if (dest != 0) {
        if (length == 0) {
            CopyVec3(dest, origin);
        } else {
            dest->x = origin->x + offset.x * distance / length;
            dest->y = origin->y + offset.y * distance / length;
            dest->z = origin->z + offset.z * distance / length;
        }
    }
    if (length >= distance) {
        result = 0;
    }
    return result;
}
