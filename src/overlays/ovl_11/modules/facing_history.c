/*
 * facing_history — reconstructed translation unit.
 *
 * Save the supplied facing and the previous saved value in shared work.
 * The direction controller calls this before choosing a new facing; this
 * does not itself change the primary object's facing or animation.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_facing_history.h"

void Ovl11PushFacingHistory(s32 facing) {
    u16 *work = (u16 *)D_8006C838;
    u16 previous = work[0x2900];

    work[0x2900] = facing;
    work[0x2901] = previous;
}
