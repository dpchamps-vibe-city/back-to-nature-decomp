#ifndef STATE_MEMORY_INIT_H
#define STATE_MEMORY_INIT_H

#include "common.h"

/* Clear the 0xE7B8/0x256E4-byte state buffers and bind their initial tables.
 * Initializes 19 pointer/count pairs for 0x18-byte records (197 total).
 * Places a 0x49000-byte reservation after the rounded-up, 2048-byte-multiple
 * D_8001009C-D_80010098 span, and points the external-input slot at
 * GetAssetScratchEnd()-0x1008. Requires the graphics heap/scratch layout first;
 * it neither clears record data nor checks available memory. */
void InitStateMemory(void);

/* Rebind the five static table pointers in the primary state buffer.
 * preservePools == 0 also restores all 19 initial pool pointers and counts;
 * any nonzero value leaves them untouched (the scene-init path passes 1).
 * Does not clear either state buffer or the records. */
void BindStateDataTables(s32 preservePools);

#endif
