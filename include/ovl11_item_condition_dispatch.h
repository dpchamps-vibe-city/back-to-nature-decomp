#ifndef OVL11_ITEM_CONDITION_DISPATCH_H
#define OVL11_ITEM_CONDITION_DISPATCH_H

#include "common.h"

/* Evaluate the recognized condition codes in 0..75 hex; unsupported codes
 * return 0. The FF "no condition" shortcut belongs to the interpreter's
 * Ovl11ConditionItemListMatches wrapper, not this entry.
 *
 * Codes consult different combinations of inventory, record kinds/amounts,
 * item values, calendar slots and game flags. Return the raw signed predicate
 * word: individual cases compare, invert or forward their provider's result.
 * Do not normalize it or assume this is a side-effect-free flag lookup:
 * grid searches and record-pair queries can update shared scratch/value slots.
 * The resident area's work records and provider tables must be initialized;
 * this dispatcher does not validate pointers or supply missing records.
 */
s32 Ovl11EvaluateItemConditionCode(s16 conditionCode);

#endif
