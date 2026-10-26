#ifndef OVL11_ITEM_SPRITE_SOURCE_H
#define OVL11_ITEM_SPRITE_SOURCE_H

#include "game_types.h"

/* Install the item's sprite header into SpriteSourceData at object+C8.
 * The object must extend through that complete 0x30-byte embedded source;
 * Ovl11ItemSpriteObject is only a partial field view. Require the ten loaded
 * sprite-record sets/blob and season 0..3 for source kind 1.
 *
 * Item zero clears the header field. A nonzero existing header suppresses
 * reinstallation; otherwise work +25476 must equal object->sourceKind.
 * Kind 1 uses its seasonal map, 4/5 use set 7, 6 uses set 4. Unsupported
 * kind/item mappings clear itemId and spriteHeader. No allocation or result
 * is returned. All table offsets and object storage must already be valid. */
void Ovl11InstallItemSpriteSource(Ovl11ItemSpriteObject *object);

#endif
