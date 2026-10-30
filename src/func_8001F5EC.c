/* Dot product of two vectors, each normalized by the SDK into a scratch
 * long-word vector first.
 * Membership evidence: notes/file-groupings.md. */

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 func_8001F5EC(VECTOR *v0, VECTOR *v1) {
    VECTOR first;
    VECTOR second;

    VectorNormal(v0, &first);
    VectorNormal(v1, &second);
    return first.vx * second.vx + first.vy * second.vy + first.vz * second.vz;
}
