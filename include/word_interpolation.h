#ifndef WORD_INTERPOLATION_H
#define WORD_INTERPOLATION_H

#include "common.h"

/* Evaluate second + (first-second)*(span-min(progress,span))/span.
 * Thus progress==0 selects first, progress==span selects second. Only the
 * upper endpoint is clamped; negative progress extrapolates for positive span.
 * Units are caller-defined signed words, not inherently pixels/colors/Q12.
 * Requires span!=0 and nonoverflowing signed intermediate arithmetic; integer
 * division truncates toward zero. No lower clamp or saturation is performed.
 * Three-word form walks exactly three elements in order, with no buffering;
 * caller must account for sequential alias effects if arrays overlap. */
void InterpolateThreeWords(s32 progress, s32 span, s32 *first, s32 *second, s32 *out);
void InterpolateWord(s32 progress, s32 span, s32 first, s32 second, s32 *out);

#endif
