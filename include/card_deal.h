#ifndef CARD_DEAL_H
#define CARD_DEAL_H

#include "common.h"

/* Scan a CardGameState at tableAddress (address-as-s32 ABI). Return 1 if any
 * of its 18 stride-14 slots has value, otherwise 0. No mutation or bounds
 * validation; the leading mode halfword is skipped. */
s32 CardTableContainsValue(s32 tableAddress, s16 value);

/* Clear all 0x108 bytes of gCardGameState, deal 18 distinct values in 0..39,
 * copy their attribute pairs and choose x/y within each column's half-open
 * ranges. Advances the shared random generator; duplicate draws retry.
 *
 * The OLD mode is inspected but not restored. If it was 3, slot [2][2] is
 * replaced after dealing by value -1, zero attributes and the game-state
 * coordinates at +9348/+92B2. All other cleared fields remain zero.
 *
 * Each row's weights are 1 + (maxX-x)^2/1000 + Rand(4), narrowed to s16.
 * The maximum comparison retains unsigned treatment of the signed x field.
 * The caller must provide the expected game-state coordinate/range tables. */
void DealCardGameTable(void);

#endif
