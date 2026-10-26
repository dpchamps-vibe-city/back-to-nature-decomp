#ifndef OVL15_SAVE_IMAGE_H
#define OVL15_SAVE_IMAGE_H

#include "common.h"
#include "ovl15_save_checksums.h"

/* ovl_15 must be resident. These operations use the shared transfer workspace;
 * its metadata is at +0x200 and state payload at +0x280. They allocate nothing,
 * set gOvl15SaveWorkBase to that payload, and are not reentrant with card I/O,
 * menu editing or sprite-resource loads. All layout offsets are byte offsets.
 *
 * Save sequence: serialize -> insert checksum slots -> write checksums.
 * Load sequence: verify checksums -> remove slots -> restore selected state.
 * The caller constructs the card header/icons and summary metadata separately.
 * Verification returns 0/-1 as described in ovl15_save_checksums.h. */

/* Snapshot selected runtime blocks and 37 event-record field sets. Clear the
 * image's transient B4/B8 record state words and +0x1CF8..+0x1F13 range.
 * Runtime state itself is not cleared. */
void Ovl15SerializeSaveImage(void);

/* Restore selected blocks and event fields; not a complete inverse of the
 * snapshot: runtime +0x20..+0x2C words and the saved 0x74-byte block at payload
 * +0x594C are not restored here. No validation or runtime reinitialization. */
void Ovl15RestoreSaveImage(void);

/* In-place 0x7F/0x80-byte record conversion using 201 overlapping memmoves.
 * Preserve the original copy lengths (initially 0x6441, decreasing each time)
 * and ensure the transfer workspace is fully available. Insert also advances
 * Rand and writes a random byte at transfer-buffer +0x7FFE; neither operation
 * computes or checks the checksums. Remove does not undo that random byte. */
void Ovl15InsertSaveChecksumSlots(void);
void Ovl15RemoveSaveChecksumSlots(void);

#endif
