#ifndef OVL11_SCENE_VALUE_COMMANDS_H
#define OVL11_SCENE_VALUE_COMMANDS_H

#include "common.h"

/* Resident overlay-11 object-script services. Slot operands index the shared
 * twenty-s32 value bank directly: callers must provide indices in [0,20).
 * Writes are sequential; overlapping output slots preserve that ordering.
 * Four-operand command/reset contracts retain their existing ignored inputs. */

/* stateSlot: 0 captures camera endpoints and sets 1; 1 interpolates using
 * tickSlot, increments it, and sets state 2 once the new tick reaches 177.
 * Returns 0 on both these paths, including that final interpolation call;
 * all other states return 1. It does NOT initialize tickSlot. A nonzero
 * resetOrbit resets the orbit on EVERY call, before reading the state.
 * Endpoints: height -800, distance start-1100, target Y start-1000, in raw
 * scene-coordinate units. Target Y endpoints narrow to s16. This shares one
 * interpolation scratch block; moves cannot run concurrently. Signed
 * interpolation products must fit s32. unused is ignored. */
s32 Ovl11StepCameraValueCommand(s16 stateSlot, s16 tickSlot,
                              s32 unused, s32 resetOrbit);

/* mode 0 recomputes the code at D_8006C838+0xE7A0 using ordered tests;
 * any other mode copies the existing code to outputSlot without recomputing.
 * Returns 1. The code's gameplay interpretation is not established here. */
s32 Ovl11UpdateConditionValueCommand(s16 mode, s16 outputSlot,
                                   s32 unused2, s32 unused3);

/* Expand the cached condition into four code slots. No range check on the
 * cached condition: its byte-table index must be valid (normal domain 0..9).
 * The first output is 0x7B7, followed by two conditional codes and a table
 * offset added to 0x7C3. Returns 1; does not refresh the classification. */
s16 Ovl11WriteConditionCodeSlots(s16 firstSlot, s16 secondSlot,
                               s32 thirdSlot, s32 fourthSlot);

/* Three signed table values; cached codes 5..9 add 5 to outputs one/three.
 * Codes >=10 select row zero; negative codes are not checked. Returns 1. */
s16 Ovl11WriteConditionTripleSlots(s16 firstSlot, s16 secondSlot, s32 thirdSlot);

/* Raw signed selected-position index, including the existing -1/-2 sentinels. */
s16 Ovl11GetSelectedPositionRecord(void);

/* Zero all twenty slots, not camera scratch or the selected-position index;
 * return 1. Program-start/load services explicitly supply four zero operands. */
s32 Ovl11ClearValueSlotsCommand(s32 unused0, s32 unused1, s32 unused2, s32 unused3);

#endif
