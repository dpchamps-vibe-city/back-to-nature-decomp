#ifndef OVL11_OBJECT_ARCHIVE_PIPELINE_H
#define OVL11_OBJECT_ARCHIVE_PIPELINE_H

#include "common.h"

/* The record-slot action table enters this dispatcher. It clears work
 * +522C/+5230, calls phase +7A74 (unchecked 0..3), then clears +4450 bit 20.
 * Phases: 0 start archive 0xC load, 3 poll, 1 execute, 2 release. Return 1
 * only from release; the other handlers return 0. Program ids, archive-offset
 * pairs, opcodes and branch indices must all be valid. Execution runs to an
 * FF end opcode or a record's yield bit, with no instruction budget.
 * Loads may block, clear GPU ordering tables and reuse asset/graphics scratch.
 * Completion resets the shared program base/cursor/index and selects phase 1.
 * These operations use shared work and are not reentrant. */
s32 Ovl11DispatchObjectLoadPhase(void);

/* Borrow the current 16-byte program-record cursor as its original 32-bit
 * address ABI. Validity depends on the current load/program state; no copy or
 * ownership transfer occurs, and execution/load completion can change it. */
s32 Ovl11GetObjectProgramCursor(void);

/* Select release phase 2, invoke the existing opcode reset (0,0,0,0), and
 * clear work +522C/+5230. The release handler runs on later dispatch. */
void Ovl11RequestObjectProgramRelease(void);

/* Return signed current-object id at the bound record's +2 if game flag 2
 * is nonzero, else 0. Zero is therefore not a unique "no object" marker. */
s32 Ovl11GetCurrentObjectId(void);

/* Service the signed bank request at shared work +4438. If game flag 2 is
 * exactly 1, load only a request other than -1 that differs from the cached
 * id. Clear both render-context background colors, wait for GPU completion,
 * clear active OTs, stop sequence 0, rewind sound slots (2,0), queue the bank
 * and drive sound transfer synchronously. The sound read buffer borrows the
 * graphics heap at +4290. Both contexts then receive context zero's saved
 * color. A serviced request becomes -1; an already-cached request remains.
 * If flag 2 is not 1, discard the request without transfer. */
void Ovl11ReloadObjectSoundBank(void);

/* Guarded sprite-bank entry points of this same module are published in
 * ovl11_guarded_sprite_bank.h; their callers do not need the program API. */

#endif
