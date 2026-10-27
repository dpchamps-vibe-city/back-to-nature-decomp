#ifndef OVL11_STATE_RECORD_COMMANDS_H
#define OVL11_STATE_RECORD_COMMANDS_H

#include "common.h"

/* These command entries retain their four-operand ABI and external linkage.
 * IDs, value-slot indices and resolved pointers are unchecked; callers must
 * supply initialized state records. Position units are not established.
 */

/* Store x/y/z as words (y is sign-extended from s16). IDs below 0x32 use
 * resolver field 0x22 after setting flag 0x800; other IDs use the 0xF8-stride
 * table at D_80077600. Always return 1.
 */
s32 Ovl11WriteStateRecordPosition(s16 recordId, s16 y, s32 x, s32 z);

/* Resolve sprite field 0x24. With forceFrame==0, change animation and return 0
 * if needed; otherwise return 1 only for sprite status bits 0x300, also clearing
 * shared work flag 0x100000. With forceFrame!=0, select animation/frame and
 * return 1 immediately. Animation selection uses its low byte, but the initial
 * equality test compares the byte against the full signed-halfword argument.
 */
s32 Ovl11StateRecordAnimationCommand(s16 recordId, s16 animation, s32 frame, s32 forceFrame);

/* Resolve flag field 0x25: zero clearFlag SETS bit 0x800, nonzero clears it.
 * Nonzero fromValueSlot reads a full word index from valueSlots[recordOrSlot].
 * Literal recordOrSlot 0x2C instead clears eligible state objects, regardless
 * of that flag. The fourth operand is unused. Always return 1.
 */
s32 Ovl11ChangeStateRecordFlag800(s16 recordOrSlot, s16 clearFlag, s32 fromValueSlot, s32 unused);

/* Compare the record's signed +9A16 value with requestedMatch (-1 selects
 * shared +52C6), optionally exporting the current value to resultSlot (-1
 * skips export). A match returns 1 and applies resetDepth: >0 calls the generic
 * record reset, >=2 sets flag 0x800, >=3 selects animation 0 at the saved frame.
 * A nonmatch returns 0. ID 0x2A visits IDs 1..0x24 and returns 1 unconditionally;
 * an enabled resultSlot is consequently overwritten on every visited record.
 */
s32 Ovl11ResetMatchingStateRecords(s16 recordId, s16 requestedMatch, s32 resultSlot, s32 resetDepth);

#endif
