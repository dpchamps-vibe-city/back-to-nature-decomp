#include "common.h"

s32 func_8001E878(void *, void *, void *);
s32 D_8005E4F0;

/* Split-file static-chain capture: the original-word census proves the
 * save/forward fingerprint and requires file scope before this function. */
CAPTURE_PREV_RET(hidden);

/* Point-in-quad test: decomposes a quad into point-in-triangle calls.
 * Args are four vertex indices into the pool at D_8005E4F0 (8-byte stride).
 * Degenerate quads (repeated corner index) reduce to a single triangle.
 * Returns 0 = hit, 1 = miss, read through the $v0 channel.
 *
 * The captured channel value (previous call's return) is dead-stored to
 * tmp[0] and re-forwarded into $v0 before every call — compiled-out
 * instrumentation residue, same fossil as func_8001E878's dead store.
 */
s32 func_8001E9F8(s32 i0, s32 i1, s32 i2, s32 i3) {
    s32 tmp[2];
    s32 saved;
    s32 temp_s1;
    s32 temp_s2;

    tmp[0] = hidden;
    saved = hidden;

    if ((i0 == i1) || (i0 == i2)) {
        hidden = saved;
        func_8001E878(D_8005E4F0 + (i2 * 8), D_8005E4F0 + (i1 * 8), D_8005E4F0 + (i3 * 8));
        return hidden;
    }
    if ((i3 == i1) || (i3 == i2)) {
        hidden = saved;
        func_8001E878(D_8005E4F0 + (i0 * 8), D_8005E4F0 + (i1 * 8), D_8005E4F0 + (i2 * 8));
        return hidden;
    }
    temp_s2 = i1 * 8;
    temp_s1 = i2 * 8;
    hidden = saved;
    func_8001E878(D_8005E4F0 + (i0 * 8), D_8005E4F0 + temp_s2, D_8005E4F0 + temp_s1);
    if (hidden == 0) {
        return 0;
    }
    hidden = saved;
    func_8001E878(D_8005E4F0 + (i3 * 8), D_8005E4F0 + temp_s1, D_8005E4F0 + temp_s2);
    return hidden;
}
