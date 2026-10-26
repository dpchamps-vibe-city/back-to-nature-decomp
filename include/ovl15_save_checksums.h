#ifndef OVL15_SAVE_CHECKSUMS_H
#define OVL15_SAVE_CHECKSUMS_H

#include "common.h"

/*
 * Save/load-overlay interface; ovl_15 must be resident.
 *
 * Both operations use the current 0x8000-byte image at gOvl15TransferBuffer,
 * with gOvl15SaveMetadata naming its record at byte offset 0x200. The
 * caller must finish packing/loading the image before using these routines;
 * neither routine performs serialization, card I/O or buffer allocation.
 *
 * Records are 0x80 bytes, not the card API's 8192-byte allocation blocks.
 * Records 5..255 have 127 payload bytes plus one XOR byte. Metadata holds
 * their combined XOR at +0x38 and its own first-127-byte XOR at +0x7F.
 * The first four records (card header/icons) are outside these checks.
 */

/* Refreshes the 251 record XOR bytes and both metadata checksum bytes.
 * Does not change other bytes. Original identity: ovl_15:0x80135AE0. */
void Ovl15WriteSaveChecksums(void);

/* Read-only: returns 0 if every check agrees, -1 at the first mismatch.
 * This checks integrity bytes, not save-version or other semantic validity.
 * Original identity: ovl_15:0x80135B68. */
s32 Ovl15VerifySaveChecksums(void);

#endif
