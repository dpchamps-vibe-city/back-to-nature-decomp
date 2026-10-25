#ifndef U16_STRING_H
#define U16_STRING_H

#include "common.h"

/* All buffers contain u16 values and terminate with 0xFFFF, not zero.
 * No operation allocates, checks capacity, or changes renderer state.
 * Input and reference buffers must remain readable through the tested
 * positions; copy/append destinations need space for the terminator too. */

/* Append source, including its terminator, to destination. Return the address
 * of the NEW terminator (not the start of destination). No overlap guarantee. */
u16 *AppendU16String(u16 *destination, u16 *source);

/* Unsigned halfword lexicographic ordering: -1, 0 or 1. */
s32 CompareU16String(u16 *left, u16 *right);

/* At each position accept input if it equals primary OR alternate. All three
 * advance together; an accepted input terminator returns zero. On mismatch
 * return -1/1 by comparison with primary alone. This is not two independent
 * whole-string comparisons, and an early reference terminator is not a stop. */
s32 CompareU16StringAgainstPair(u16 *input, u16 *primary, u16 *alternate);

#endif
