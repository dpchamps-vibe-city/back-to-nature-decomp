/*
 * object_work_pair_reset — reconstructed translation unit.
 *
 * Reset the five pairs of object-program work halfwords to their -1 sentinel.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_object_load_resets.h"

void Ovl11ResetObjectWorkPairs(void) {
    char *work;
    s16 *pairEnd;
    s32 pair;
    s32 sentinel;

    sentinel = -1;
    work = (char *)&D_8006C838;
    pairEnd = (s16 *)(work + 0x443C);
    for (pair = 0; pair < 5; pair++) {
        pairEnd[-1] = sentinel;
        pairEnd[0] = sentinel;
        pairEnd += 2;
    }
}
