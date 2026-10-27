#ifndef OVL11_RANDOM_VALUE_SLOT_COMMAND_H
#define OVL11_RANDOM_VALUE_SLOT_COMMAND_H

#include "common.h"

/* ovl_11:800E78E4. Advance Rand once using limit's low 16 bits; store the
 * result in gOvl11ValueSlots[slot] unless slot==-1. All other signed slot
 * indices are unchecked. A zero effective limit still advances the RNG and
 * yields zero. Then compare the generated value to operand: condition
 * 0..4 means >=, >, <=, <, ==. Unknown conditions return true.
 * Return that 0/1 result to the object-program interpreter. The comparison
 * and RNG advance occur even when the store is suppressed. Non-reentrant:
 * the RNG and optional destination are shared state. */
s32 Ovl11RandomValueSlotCommand(s32 limit, s16 slot, s32 condition, s32 operand);

#endif
