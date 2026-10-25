/*
 * Asset-loading scratch bounds, reconstructed from the boot/main-loop family.
 * Paired loader uses and a shared end boundary support this query slice, not
 * an original TU boundary here; see notes/file-groupings.md.
 */

#include "common.h"
#include "asset_scratch.h"

/* Shared boot/render state, not private storage: the graphics-heap initializers
 * set this base. A tentative definition retains the witnessed GP accesses. */
s32 D_8005E3B0;

s32 GetAssetScratchCapacity(void) {
    s32 reservationAdjustment;
    s32 remainingBytes;

    /* The lower boundary is D_8007AFF4 + 0x49000 + D_8005E328:
     * boot sets the first address, the loader records the variable byte count.
     * Keep the staged differences around D_8001009C: algebraically cancelling
     * that reference changes the original load/arithmetic sequence. */
    reservationAdjustment = -0x49000;
    remainingBytes = D_8005E3B0 - D_8001009C;
    remainingBytes = (remainingBytes + reservationAdjustment) - D_8005E328;
    return remainingBytes - (D_8007AFF4 - D_8001009C);
}

s32 GetAssetScratchStart(void) {
    return D_8005E3B0 - GetAssetScratchCapacity();
}

s32 GetAssetScratchEnd(void) {
    return D_8005E3B0;
}
