#ifndef OVL11_GRID_ENTRY_TRANSITIONS_H
#define OVL11_GRID_ENTRY_TRANSITIONS_H

#include "common.h"
#include "game_types.h"

/* Set kind/value to 0x167 and clear the two state bytes and flags. */
void Ovl11ResetGridEntry(Ovl11GridResetEntry *entry);

/* For kinds 0x167..0x169, change kind/value to 0x40/0x167 on a random hit.
 * Rand bounds by season code are {128,64,128,disabled}; other codes disable.
 * State bytes and flags are left unchanged. */
void Ovl11TrySeasonalGridTransition(Ovl11D124Entry *entry);

/* The next three take a nonzero gate whose LOW 16 BITS are passed to Rand;
 * zero disables the operation. Callers must supply a valid random bound.
 * All mutate in place, return no status and may consume the shared RNG. */

/* Kind 0x3F -> 0x175 on a hit; no other entry fields are accessed. */
void Ovl11TryGridKind3FTransition(u16 *kind, s32 randomBound);

/* For non-0x36 kinds accepted by 800D5868, reset kind/value to 0x168 and
 * clear both state bytes and flags on a random hit. */
void Ovl11TryResetEligibleGridEntry(Ovl11GridResetEntry *entry, s32 randomBound);

/* Kind 0x36 with a nonzero first state byte: on a hit, refresh that byte
 * from 800D5C90(0x36), clear the second byte and OR 0x8000 into flags. */
void Ovl11TryRefreshActiveGridEntry(Ovl11GridResetEntry *entry, s32 randomBound);

#endif
