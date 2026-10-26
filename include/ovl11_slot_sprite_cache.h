#ifndef OVL11_SLOT_SPRITE_CACHE_H
#define OVL11_SLOT_SPRITE_CACHE_H

#include "common.h"

/* Two active rows of nine IDs, followed in resident RAM by two rows of nine
 * 0x1E-byte descriptors. Zero IDs are free; reset fills descriptors with FF.
 * Existing broader array views are not a claim of additional cache rows.
 * All storage remains external. Call reset before installation/drawing. */
void Ovl11ResetSlotSpriteCache(void);

/* Classify the ID, folding class 2 into row 1, and remove its first match.
 * ID zero is ignored. Does not erase or release the corresponding VRAM. */
void Ovl11RemoveSlotSprite(s16 id);

/* Insert into the first zero slot, returning its index or -1 when full.
 * Does not check duplicates, initialize the descriptor or reject ID zero.
 * row must be 0 or 1. */
s32 Ovl11InsertSlotSpriteId(s16 value, s16 row);

/* CLUT x >= 1008 prevents shared placement; -1 denotes an empty entry.
 * Rank search returns the first unused texture-u rank 0..7, or 0 if full.
 * These helpers read descriptors only and do not reserve VRAM themselves. */
s32 Ovl11CanUseSharedSlotClut(s16 row);
s32 Ovl11FindFreeSlotTextureRank(s16 row);

/* Return a resident sprite header address, or zero if the current area's
 * ID/offset tables have no match. This is an integer-address ABI. */
s32 Ovl11FindResidentSlotSprite(s16 id);

/* Return 0 for ID zero or a full ID row; otherwise reserve a slot and install
 * its two image variants. Prefer the resident sprite blob, otherwise perform
 * a blocking 2048-byte disc read at 03C19000 + id*2048 into message scratch.
 * Copies/upload modify VRAM; do not overlap message loading or cache users.
 * The resource must have the primary piece needed by its secondary variant.
 * Failure after reservation is not rolled back; no duplicate-ID check. */
s32 Ovl11InstallSlotSprite(s16 id);

/* Draw the first ID match, row 0 taking priority within each slot index.
 * alternate != 0 selects the second variant. x/y are pixels. Flags: bit 1
 * adds the variant's stored offsets; bit 2 adds (16,16) and enables bit 1;
 * bit 3 centers the PRIMARY dimensions in a 32-pixel cell. Any of bits
 * 0/2/3 also invokes the separate background helper at ot+1. Links SPRT and
 * DR_MODE to ot and advances packet_work; caller provides packet capacity.
 * Missing IDs draw nothing. Does not animate the cached source. */
void Ovl11DrawSlotSprite(s16 id, s16 alternate, s32 *ot, s16 x, s16 y, s32 flags);

#endif
