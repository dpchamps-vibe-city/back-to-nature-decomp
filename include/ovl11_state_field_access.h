#ifndef OVL11_STATE_FIELD_ACCESS_H
#define OVL11_STATE_FIELD_ACCESS_H

#include "common.h"

/* IDs 0..47 select gOvl11StateFields; IDs 0x31..0x31F address engine flags
 * (id-0x30). Other IDs and element indices are unchecked. Descriptor stride is
 * bytes: zero means scalar, otherwise elementIndex selects a record.
 * Reads use u8/u16/s16/s32 according to the descriptor, collapse a nonzero mask
 * to boolean, and add one for IDs 0xC/0xE only.
 * Reads leave state unchanged. The condition adapter and operand commands
 * share this two-input contract; the adapter does not narrow either input. */
s32 Ovl11ReadStateField(s32 fieldId, s32 elementIndex);

/* Masked descriptors set/clear their bits according to operand!=0, ignoring
 * operation. Unmasked fields apply operation, clamp to signed min/max, then
 * truncate to the access width. Engine flag writes bypass descriptors. */
void Ovl11UpdateStateField(s32 fieldId, s32 elementIndex, s32 operand, s32 operation);

/* Rule comparisons: conditions 0..4 are >=, >, <=, <, == respectively.
 * Unknown codes return true; the result is always 0 or 1. */
s32 Ovl11TestStateFieldCondition(s32 value, s32 operand, s32 condition);

/* Resolve descriptor IDs only. kindOut may be null. A null data pointer enters
 * the resumable breakpoint loop; there is no recoverable error return. */
u8 *Ovl11ResolveStateField(s32 fieldId, s32 elementIndex, s32 *kindOut);

/* Operations: 0 assign, 1 add, 2 subtract, 3 multiply, 4 divide, 5 modulo.
 * Division/modulo by zero and unknown operations leave the value unchanged.
 * Returns 1 in all cases. */
s32 Ovl11ApplyStateFieldOperation(s32 *value, s32 operand, s32 operation);

#endif
