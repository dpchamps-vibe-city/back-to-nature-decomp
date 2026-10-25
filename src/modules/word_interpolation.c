/* Signed-word interpolation shared by color/vector consumers. Scalar and
 * three-word forms use the same upper clamp and multiply/divide order.
 * No persistent state or GPU-specific layout is owned here.
 * Membership/boundary evidence: notes/file-groupings.md. */
#include "word_interpolation.h"

void InterpolateThreeWords(s32 progress, s32 span, s32 *first, s32 *second, s32 *out) {
    s32 i;
    s32 *cursor;

    if (span < progress) {
        progress = span;
    }
    progress = span - progress;

    cursor = out;
    i = 2;
    do {
        *cursor++ = (*first - *second) * progress / span + *second;
        i--;
        first++;
        second++;
    } while (i >= 0);
}

void InterpolateWord(s32 progress, s32 span, s32 first, s32 second, s32 *out) {
    s32 remaining;

    if (span < progress) {
        progress = span;
    }
    remaining = span - progress;
    *out = (first - second) * remaining / span + second;
}
