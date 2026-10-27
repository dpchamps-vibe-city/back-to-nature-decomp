#ifndef OVL11_OBJECT_PROGRAM_BRANCH_SLOTS_H
#define OVL11_OBJECT_PROGRAM_BRANCH_SLOTS_H

#include "common.h"

/* Require a valid current object-program cursor and unchecked value-slot
 * index. These are opcode handlers; linkage stays external for the original
 * dispatch table. The interpreter treats a handler result of 1 as success.
 */

/* Export the current record's failure-target byte (+2). If it is FF, scan
 * the 206-record resident array and store current index + 1 (the sequential
 * next target, not a one-based encoding for explicit branches). Returns 1
 * after writing; if the FF cursor is not found, returns 0 without writing.
 * Does not validate explicit targets or guard sequential target 206. */
s32 Ovl11StoreProgramFailureTargetToSlot(s16 destinationSlot);

/* Store the LOW BYTE of the selected s32 value slot into the current
 * record's success-target byte (+1); FF selects sequential execution.
 * No range check or cursor advance; returns 1. The interpreter reads this
 * possibly modified target immediately after the opcode handler returns. */
s32 Ovl11SetProgramSuccessTargetFromSlot(s16 sourceSlot);

#endif
