#ifndef OVL11_RECORD_POSITION_RECOVERY_H
#define OVL11_RECORD_POSITION_RECOVERY_H

#include "common.h"

/* Reset blocked records in the two singleton pools and ALL 20 B8/10 B4
 * entries, independent of expansion settings. Query mask is 0x30. State 1
 * stays 1; eligible state 6 becomes 1. Zero gates can force that transition
 * without a query. Other states and clear positions are unchanged.
 *
 * Fallback XYZ, in existing raw coordinate units:
 *   singleton +91AC: (-1800,0,3100), +929C: (5000,0,2700);
 *   B8 index i: (400*i-1000,0,2200); B4: (-5000,0,400*i-7600).
 *
 * Requires initialized pool storage and enough shared collision budget for
 * up to 32 scans. Output pointers are null: the budget-exhaustion path of
 * Ovl11QueryCollisionBudgeted would write through null. This is not a safe
 * fallback for an already exhausted frame budget.
 */
void Ovl11RecoverRecordPositions(void);

#endif
