/* Export the four resident layout-selection halfwords to object-program
 * value slots. The area resolver uses these as primary/secondary variants;
 * the middle two also select expanded record pools. Values are not normalized.
 * No direct C caller exists; the externally linked entry is an opcode handler.
 * Membership and boundaries: notes/file-groupings.md.
 */
#include "common.h"

/* Each -1 suppresses only its own write. Other signed indices are unchecked;
 * arguments 1/2 are s16, arguments 3/4 are full words. Writes occur in order,
 * so later outputs win when slot indices alias. Always returns 1, even when
 * all outputs are discarded. No source selector or pool is modified. */
s32 Ovl11StoreLayoutStateToSlots(s16 primarySlot, s16 taggedPoolSlot,
                                s32 b4PoolSlot, s32 indexedVariantSlot) {
    if (primarySlot != -1) {
        gOvl11ValueSlots[primarySlot] = gAreaPrimaryVariant;
    }
    if (taggedPoolSlot != -1) {
        gOvl11ValueSlots[taggedPoolSlot] = gTaggedRecordPoolExpanded;
    }
    if (b4PoolSlot != -1) {
        gOvl11ValueSlots[b4PoolSlot] = gB4RecordPoolExpanded;
    }
    if (indexedVariantSlot != -1) {
        gOvl11ValueSlots[indexedVariantSlot] = gAreaIndexedVariant;
    }
    return 1;
}
