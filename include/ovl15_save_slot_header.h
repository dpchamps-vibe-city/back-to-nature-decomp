#ifndef OVL15_SAVE_SLOT_HEADER_H
#define OVL15_SAVE_SLOT_HEADER_H

#include "common.h"

/* Capture current summary words, two nine-halfword names and eight work bytes
 * into slot's 0x3C-byte record. Set its +34 marker to 0x14; retain +38.
 * Then clear ALL 0x9690 transfer-workspace bytes and copy this record to +200.
 * Must precede payload serialization/icon setup that needs that workspace.
 * slot must index the three summary records (0..2); no validation occurs.
 * Other slot records are unchanged. This does not build the card title,
 * serialize the full runtime state or compute/save any checksum. */
void Ovl15BuildSaveSlotHeader(s16 slot);

#endif
