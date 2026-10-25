#ifndef OVL17_RECORD_SPRITE_ANIMATION_H
#define OVL17_RECORD_SPRITE_ANIMATION_H

#include "common.h"
#include "game_types.h"

/* The sprite source is embedded at record+0x20. If animation N is already
 * selected, leave its frame/timer untouched; otherwise select N and reset
 * its frame/timer through SelectSpriteAnimation. The numeric animation IDs
 * describe the data, not an inferred gameplay state. Records are unchecked.
 *
 * Single-record calls retain the matched s32 ABI: they return N on the
 * already-selected path and have NO defined return value on the other path.
 * All observed callers discard the result; callers must continue to do so. */
s32 Ovl17SelectRecordAnimation2(Ovl17RecordAnimationView *record);
s32 Ovl17SelectRecordAnimation0(Ovl17RecordAnimationView *record);
s32 Ovl17SelectRecordAnimation1(Ovl17RecordAnimationView *record);

/* Visit six records from gOvl17AnimatedRecordBase in increasing address order,
 * 0x50-byte stride. No bounds check, animation advance or drawing. */
void Ovl17SelectAllRecordAnimations2(void);
void Ovl17SelectAllRecordAnimations0(void);

#endif
