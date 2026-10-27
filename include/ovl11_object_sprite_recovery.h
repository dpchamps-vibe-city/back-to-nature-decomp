#ifndef OVL11_OBJECT_SPRITE_RECOVERY_H
#define OVL11_OBJECT_SPRITE_RECOVERY_H

#include "common.h"

/* Original ovl_11:800E54F8. Recover resource 0x15F..0x190 from the uploaded
 * sprite-block bank, using the guarded uploader's retained VRAM-word X
 * offset and Y offset zero. This is StoreImage readback, NOT an archive read.
 * The underlying service rejects other indices with NULL.
 *
 * Requires a matching uploaded bank. Scratch is word-aligned RAM large enough
 * for covering VRAM rows; the borrowed result can instead point into the
 * bank's retained asset-scratch tail. Preserve both stores until consumed.
 * Readback/copy can modify scratch even when a bad checksum returns NULL.
 * See ovl11_sprite_block_bank.h for the bank lifetime/capacity constraints. */
u8 *Ovl11RecoverObjectSpriteBlock(u16 *scratch, s32 resourceId);

#endif
