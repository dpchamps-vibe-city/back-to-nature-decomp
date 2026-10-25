#ifndef OVL11_GUARDED_SPRITE_BANK_H
#define OVL11_GUARDED_SPRITE_BANK_H

#include "common.h"

/* Both operations do nothing if game flag 2 is exactly 1 or work bits
 * 0x18000 are set. Reload waits for the GPU, clears the active secondary OT
 * (2048 entries), reads archive range [0x1D, 0x1E) into the configured buffer,
 * and invokes Upload. It always returns 0, not an upload success indication. */
s32 Ovl11ReloadGuardedSpriteBank(void);

/* Upload an already loaded sprite-bank buffer. Stage 44 suppresses the call.
 * The bank rectangle's x offset is 0 or 256 VRAM words, selected by bit 15
 * of the current stage record (+8); stage 27 always selects 0. The successful
 * path stores this offset for subsequent sprite-bank reads. The lower-level
 * uploader builds bank entries, issues LoadImage and waits for completion. */
void Ovl11UploadGuardedSpriteBank(u8 *buffer);

#endif
