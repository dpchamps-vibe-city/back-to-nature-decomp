#ifndef OVL11_ITEM_INTERACTION_MAP_H
#define OVL11_ITEM_INTERACTION_MAP_H

#include "common.h"

/* Translate a signed item ID to the first matching interaction ID in the
 * fourteen-pair table. Returns 0 when absent; changes no state. The caller
 * decides whether to pass the result to Ovl11BeginInteraction. */
s32 Ovl11GetItemInteractionId(s16 itemId);

#endif
