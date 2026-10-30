/* Classify a 2D determinant: 1 for positive, 0 for negative, 3 for zero.
 * Membership evidence: notes/file-groupings.md. */

#include "common.h"

s32 func_8001F5B8(s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 determinant;
    s32 classification;

    determinant = x0 * y1 - y0 * x1;
    classification = determinant >= 0;
    if (determinant == 0) {
        classification |= 2;
    }
    return classification;
}
