#ifndef OVL11_SPRITE_EFFECT_POOL_H
#define OVL11_SPRITE_EFFECT_POOL_H

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

/* Resident ovl_11: 25 globally shared projected sprite effects.
 * Clear releases every entry by nulling its sprite; other bytes remain.
 * Update draws/steps all entries unless work flag 0x400 suppresses the batch.
 * Work flag 0x08000000 freezes ticks, not drawing or end-of-animation release.
 * A frame lasts three eligible updates; the stored tick wraps at 16 bits and
 * is interpreted signed for division. No elapsed-time or frame-rate scaling.
 * Original entries: 801212C4, 80121318, 8012135C. */
void Ovl11UpdateSpriteEffects(void);
void Ovl11ClearSpriteEffects(void);

/* First free entry, no eviction: 0 if full, otherwise 1 after initialization.
 * id is masked to 16 bits. Position is an SDK VECTOR passed by value, including
 * its unused fourth word; only x/y/z are stored. Initialization selects art
 * and animation from id, season and area. Some id/season combinations leave
 * the entry free even though Spawn returns 1 (not a promise of visible output).
 * Drawing truncates coordinates to unsigned 16 bits, offsets (+150,-100,-150)
 * and projects into the current ordering table. Caller coordinates therefore
 * must obey that existing projection convention; no bounds checks are added.
 * The reserved effect word is untouched. Shared state is not reentrant. */
s32 Ovl11SpawnSpriteEffect(s32 id, VECTOR position);

#endif
