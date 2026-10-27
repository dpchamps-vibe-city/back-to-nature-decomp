/*
 * Read-only item-to-interaction mapping (ovl_11:80118C28–80118C6C).
 * The nearby-item volume action uses the result to begin an interaction;
 * lookup itself changes no interaction or item state. The fourteen-pair
 * table is independent of the neighboring prompt and reset/getter services.
 * Membership and evidence-limited boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_item_interaction_map.h"

s32 Ovl11GetItemInteractionId(s16 itemId) {
    Ovl11ItemInteractionEntry *entry = gOvl11ItemInteractionMap;
    u32 pairIndex;

    for (pairIndex = 0; pairIndex < 14U; pairIndex++) {
        if (entry->itemId == itemId) {
            return entry->interactionId;
        }
        entry++;
    }
    return 0;
}
