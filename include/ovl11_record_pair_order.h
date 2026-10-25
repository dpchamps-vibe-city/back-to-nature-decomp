#ifndef OVL11_RECORD_PAIR_ORDER_H
#define OVL11_RECORD_PAIR_ORDER_H

#include "common.h"
#include "game_types.h"

/* ovl_11 must be resident. No private mutable state or pointer ownership.
 * Both parameter types are existing partial views of the caller's record;
 * the selector explicitly converts to the classifier's four-word view. */

/* Compare signed words (+0x38 vs +0x58) and (+0x40 vs +0x60). Each rank is
 * 0 equal, 1 less, 2 greater. Return table[firstRank][secondRank]:
 *     { 0,5,7 }, { 6,1,2 }, { 8,4,3 }.
 * Original identity: ovl_11:0x800D062C. The comparator at 800D0600 remains
 * nested in this function and is not a separately callable public API. */
u16 Ovl11ClassifyRecordPairOrder(Ovl11RankPair *record);

/* Compare abs(s32@38 - s32@58) against abs(s32@3C - s32@5C).
 * If the LATTER is smaller, class pairs (1,5)/(2,6)/(3,7)/(4,8) select
 * codes 3/3/1/1; otherwise they select 2/0/0/2. Store the s16 code at +0x22.
 * Class 0 (and unknown classes) leaves it unchanged. Note +0x3C here versus
 * +0x40 in classification: this difference is intentional, not normalized.
 * The original abs/subtraction arithmetic is retained; callers must avoid
 * signed overflow, including abs(INT_MIN). No gameplay direction is inferred.
 * Original identity: ovl_11:0x800D0698. */
void Ovl11SelectRecordPairCode(Recon_ovl_11_func_800D2594_A0View *record);

#endif
