#ifndef OVL11_SPRITE_BLOCK_BANK_H
#define OVL11_SPRITE_BLOCK_BANK_H

#include "common.h"

/* Original ovl_11:800E351C. Compute fifty block word sums, upload the
 * (320,0,128,255) rectangle plus x/y offsets to VRAM, then wait for GPU.
 * Coordinates/width are VRAM halfwords; height is rows. Source must contain
 * the complete table-defined bank, word-aligned with word-sized blocks.
 * Overflow beyond the rectangle is copied to an aligned-down address at the
 * asset scratch end, only if capacity permits and game flag 2 is not set.
 * Otherwise the old scratch-base value remains; 0 is returned in every case.
 * This operation replaces the bank/checksums and may overwrite asset scratch. */
s32 Ovl11UploadSpriteBlockBank(u8 *source, s16 xOffset, s16 yOffset);

/* Original ovl_11:800E36CC. RECOVER (StoreImage), not upload, block 0..49.
 * Scratch must be word-aligned and large enough for complete covering VRAM
 * rows plus any overflow tail. GPU readback is synchronous. Result is either
 * inside caller scratch or the retained asset-scratch tail; it is borrowed,
 * not newly allocated. Keep both stores alive until the result is consumed.
 * Requires the matching bank upload and x/y offsets, and a valid retained
 * overflow tail when needed. Returns NULL for an invalid index or bad sum;
 * readback/copy may already have modified scratch on a checksum failure. */
u8 *Ovl11ReadSpriteBlock(u16 *scratch, s16 xOffset, s16 yOffset, u32 blockIndex);

/* Original ovl_11:800E3978. Byte size for resource ids 0x15F..0x190,
 * otherwise 0. Readback instead accepts the zero-based block index. */
s32 Ovl11GetSpriteBlockSize(s32 resourceId);

#endif
