#ifndef OVL11_INTERACTION_VOLUME_QUERY_H
#define OVL11_INTERACTION_VOLUME_QUERY_H

#include "common.h"
#include "game_types.h"

/* Both queries scan 50 records at stride 0x2C and return record+0x10, or null.
 * Bounds are strict (points on a face miss). Records and points are borrowed.
 * The first query uses s32 X/Z at point+0/+8, ignores Y, and rejects empty
 * type 0xFFFF or flags 0x6008. Flag-0x100/0x80 hits are skipped; when
 * applyEffects!=0 they request event 7/set work bit 4. A returned hit resets
 * the pending selector unless its type is 0xFFFE, even with applyEffects==0. */
void *Ovl11ProbeInteractionVolume(u8 *volumes, Ovl11Func800BDCE8Arg1 *point,
                                s32 applyEffects);

/* Pure lookup of flag-0x4000 records using s16 XYZ. Y bounds are widened by
 * 0x80 below and 0x78 above; X/Z use the stored bounds unchanged. */
void *Ovl11FindFlaggedVolume(u8 *volumes, s16 *point);

#endif
