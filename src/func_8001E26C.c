/* Bounded Euclidean length of a long vector. Each component is range-checked
 * before the vector is accepted; a rejection returns -1, otherwise the
 * squares accumulated by the GTE are summed and rooted.
 * Membership evidence: notes/file-groupings.md. */

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/inline_c.h"
#include "psyq/inline_c_gas.h"

s32 func_8001E26C(VECTOR *vector, s32 limit) {
    VECTOR squares;

    squares.vx = vector->vx;
    squares.vy = vector->vy;
    squares.vz = vector->vz;
    if (squares.vx > limit || squares.vx < -limit) {
        return -1;
    }
    gte_ldlvl(&squares);
    if (squares.vy > limit || squares.vy < -limit) {
        return -1;
    }
    gte_sqr0_b();
    if (squares.vz > limit || squares.vz < -limit) {
        return -1;
    }
    gte_stlvnl(&squares);
    return SquareRoot0(squares.vx + squares.vy + squares.vz);
}
