#ifndef HORSE_RACE_SETUP_H
#define HORSE_RACE_SETUP_H

#include "common.h"

/* Scan a HorseRaceState at tableAddress (address-as-s32 ABI). Return 1 if
 * horseId occurs in any of its 18 stride-14 entries, otherwise 0. No mutation
 * or bounds validation; the leading mode halfword is skipped. */
s32 HorseRaceContainsEntrant(s32 tableAddress, s16 horseId);

/* Clear all 0x108 bytes of gHorseRaceState and initialize three races of six
 * entrants with distinct horse IDs in 0..39. Copy their attribute pairs and
 * randomize two race parameters in per-entrant half-open ranges. Advances
 * the shared random generator; duplicate horse draws retry.
 *
 * The OLD mode is inspected but not restored. If it was 3, entry [2][2] is
 * replaced by the player's horse: ID -1, zero attributes and parameters from
 * game-state +9348/+92B2. Other fields remain zero unless initialized here.
 *
 * Odds are 1 + (maxParameter0-parameter0)^2/1000 + Rand(4), narrowed to s16.
 * The maximum comparison treats the signed parameter as unsigned. Exact
 * meanings of the two parameters and attribute halfwords remain unresolved.
 * ovl_17 displays odds and ticket counts and uses their product as winnings. */
void InitHorseRaceEntries(void);

#endif
