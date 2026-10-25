/*
 * interaction_id_predicates — reconstructed translation unit.
 *
 * Three item-ID membership tests used by neighboring interaction-prompt
 * wrappers. The sets are documented explicitly; gameplay categories are not
 * inferred.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_interaction_id_predicates.h"

s32 Ovl11IsInteractionItemSetA(s16 itemId) {
    if (((u32) (itemId - 0x122) < 3U) || (itemId == 0xE8) || (itemId == 0x125)) {
        return 1;
    }
    return 0;
}

s32 Ovl11IsInteractionItemSetB(s16 itemId) {
    if (((u32) (itemId - 0x8B) < 3U) || (itemId == 0xE3)) {
        return 1;
    }
    return 0;
}

s32 Ovl11IsInteractionItemSetC(s16 itemId) {
    if (((u32) (itemId - 0x91) < 3U) || (itemId == 0xE7)) {
        return 1;
    }
    return 0;
}
