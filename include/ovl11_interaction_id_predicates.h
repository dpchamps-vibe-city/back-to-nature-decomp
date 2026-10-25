#ifndef OVL11_INTERACTION_ID_PREDICATES_H
#define OVL11_INTERACTION_ID_PREDICATES_H

#include "common.h"

/* Exact signed-halfword item-ID membership tests, returning 0 or 1.
 * Names deliberately leave the gameplay categories unknown. The adjacent
 * interaction wrappers use A/B/C to select action/message pairs
 * 0x5B/0x1A2, 0x59/0x1A3 and 0x5A/0x1A4 respectively. No state changes. */
s32 Ovl11IsInteractionItemSetA(s16 itemId); /* 0x122..0x125 or 0xE8 */
s32 Ovl11IsInteractionItemSetB(s16 itemId); /* 0x8B..0x8D or 0xE3 */
s32 Ovl11IsInteractionItemSetC(s16 itemId); /* 0x91..0x93 or 0xE7 */

#endif
