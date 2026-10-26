#ifndef OVL11_RANDOM_STATE_CHOICE_H
#define OVL11_RANDOM_STATE_CHOICE_H

#include "common.h"

/*
 * Read the object's word at +0x34 (requires at least 0x38 readable bytes) and
 * advance the shared random generator exactly once. Bit 0x2000 widens the
 * roll range from 3 to 8 for the three-way choices, or from 2 to 7 for the
 * two-way choices. Only roll 0 (and roll 1 in the three-way choices) selects
 * a special state; all other rolls select state 1. No object fields change.
 * The bit's gameplay meaning is not established.
 */
s32 Ovl11RollState8(void *object);  /* 0 -> 8, 1 -> 9, else 1. */
s32 Ovl11RollState11(void *object); /* 0 -> 11, 1 -> 12, else 1. */
s32 Ovl11RollState10(void *object); /* 0 -> 10, else 1. */
s32 Ovl11RollState13(void *object); /* 0 -> 13, else 1. */

#endif
