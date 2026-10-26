#ifndef OVL23_SPRITE_RESOURCES_H
#define OVL23_SPRITE_RESOURCES_H

/*
 * Sprite/image/sound bring-up stages used in this order by ovl_23's initializer.
 * Sprite loads block by polling CD requests to completion, clear the active
 * ordering table, and share graphics-heap scratch at +0x4290. Not reentrant;
 * the graphics heap, archive index and active render context must already exist.
 * Data stays at its original overlay addresses; no storage is allocated here.
 */

/* Entry 0, bytes 0x25000..0x2A800: upload images, copy a 0x13DC-byte header
 * bank both locally and to gSharedSpriteData, initialize one local source. */
void Ovl23LoadSharedSpriteBank(void);

/* Entry 0, first 0x2000 bytes: image upload only. */
void Ovl23LoadSpriteImageBank(void);

/* Entry 0x18, first 0x1EC1C bytes: upload images and copy the 0x29A8-byte
 * header bank. Initialize the scene source and both six-record source arrays.
 * The 0x44-stride array's record 2 selects offset-table slot 1; others slot 2.
 * The 0x50 array uses slot 0 and the scene source slot 3. Tables are unchecked. */
void Ovl23LoadActorSpriteBank(void);

/* Rewind sound-load slots 2/1 and queue bank commands 0x3E7/0x12 into 0/1.
 * Returns before transfer completion; the caller performs the readiness wait. */
void Ovl23QueueSoundBankLoads(void);

/* Set/clear mask 4 on the twelve per-record sources only. This bit suppresses
 * the entire animation update; it does not hide their already selected frames.
 * Requires initialized sources. Neither function changes the scene source. */
void Ovl23PauseActorSprites(void);
void Ovl23ResumeActorSprites(void);

/* Select scene animation 5, then draw its current frame at (0,0), without
 * advancing it. Uses the active context's field_D8+4 ordering table. */
void Ovl23DrawPausedSpriteFrame(void);

#endif
