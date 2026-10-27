#ifndef OVL11_HELD_ITEM_FLAGS_H
#define OVL11_HELD_ITEM_FLAGS_H

#include "game_types.h"

/* Synchronize object +6C bits 08000000/10000000/00008000 from itemIndex
 * and gItemDataTable[itemIndex].u0.flags & 0180. Zero clears all three and
 * resets the GLOBAL primary-object request, even for a non-primary object.
 * Nonzero sets exactly one category bit and the held-item bit. Other bits
 * are preserved. The nonzero item index is unchecked; no sprite is loaded. */
void Ovl11SyncHeldItemFlags(Ovl11C7010Object *object);

#endif
