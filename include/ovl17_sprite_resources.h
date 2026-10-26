#ifndef OVL17_SPRITE_RESOURCES_H
#define OVL17_SPRITE_RESOURCES_H

#include "common.h"

/* Resource stages called in this order by the scene initializer.
 * Sprite loaders synchronize drawing, clear 0x800 OT entries, then busy-poll
 * archive reads in the graphics heap at +0x4290. No timeout/error escape is
 * provided. Do not run them while another user needs that scratch storage.
 * UploadSpriteImages handles the archive's image packets/VRAM transfers. */
/* Archive 0, byte window [0x25000,0x2A800): upload images and retain 0x13DC
 * bytes in both the overlay's shared bank and the engine's shared sprite data.
 * Initializes the overlay shared-bank sprite source. */
void Ovl17LoadSharedSpriteBank(void);
/* Archive 0, first 0x2000 bytes: upload images only. */
void Ovl17LoadSpriteImageBank(void);
/* Archive 0x12, first 0x1E0A8 bytes: retain the 0xC28-byte record bank,
 * publish its two header variants and bind scene/display/record sources.
 * The variant pointers remain valid only while this overlay bank is resident. */
void Ovl17LoadRecordSpriteBank(void);
/* Rewind sound-load slots 2 and 1; queue commands 0x3E7/0 and 0x15/1.
 * This queues loading; the initializer separately waits for completion. */
void Ovl17QueueSoundBankLoads(void);

#endif
