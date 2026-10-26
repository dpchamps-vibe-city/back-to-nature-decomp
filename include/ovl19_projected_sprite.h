#ifndef OVL19_PROJECTED_SPRITE_H
#define OVL19_PROJECTED_SPRITE_H

#include "common.h"
#include "game_types.h"

/* Both paths first MODIFY position[0..2] through AdjustPositionToSurface,
 * then project signed world coordinates and submit sprite quads. orderingTable
 * is a 32-bit OT base address; depth chooses four-byte slots (depth >> 3,
 * or the general sprite path's conditional depth >> 2). screenYOffset is in
 * pixels. The active render context receives the companion sprite submission.
 *
 * These routines do not reject a zero projection result or check OT bounds.
 * They update sprite animation state, use shared projection scratch and require
 * initialized scene geometry, render context and sprite sources. Not reentrant.
 * The +0x120 actor-sprite source bypasses the companion and depth offset; the
 * three main actor sources apply the Y offset to both sprite submissions. */
void Ovl19DrawProjectedSprite(s32 orderingTable, SpriteSourceData *sprite,
                              s16 *position, s16 screenYOffset);

/* Selects animation & 0xFF before drawing, with no pixel Y adjustment. */
void Ovl19DrawProjectedSpriteState(s32 orderingTable, ObjectState *sprite,
                                   s16 *position, s16 animation);

#endif
