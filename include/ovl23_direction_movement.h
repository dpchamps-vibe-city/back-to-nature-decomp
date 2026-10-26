#ifndef OVL23_DIRECTION_MOVEMENT_H
#define OVL23_DIRECTION_MOVEMENT_H

#include "common.h"
#include "game_types.h"

/*
 * All entries require record->index in 0..5 and initialized arena bounds/
 * anchors. Planar positions and stored speed use signed Q12; geometry converts
 * to integer units by truncating toward zero. Direction codes 0..7 are +Y,
 * +Y+X, +X, -Y+X, -Y, -Y-X, -X, +Y-X; diagonal steps use speed*1000/1414.
 */

/* In state +4 == 1 use speed at +0x10; in state 3 use arena +0x3B2/10.
 * Other states do nothing. If clearance allows it, update x/y in place and
 * clamp X to [-48, arena.s16@+0x39C + 55] integer units. No RNG consumption. */
void Ovl23StepRecordInDirection(Ovl23PositionedRecord *record);

/* Direction of the vector FROM this record's anchor TO its live position;
 * no state changes or randomness. SDK angle becomes integer degrees, then
 * one of eight 45-degree sectors (first boundary 23 degrees). */
s32 Ovl23DirectionFromRecordAnchor(Ovl23PositionedRecord *record);

/* Start with that anchor-relative direction. Test clearance at 3/2 the arena
 * radius; if blocked, add either 2 or 4 modulo 8 using one Rand(2). Does not
 * write the record. Retains the original discarded SquareRoot0 call. */
s32 Ovl23ChooseRecordDirection(Ovl23PositionedRecord *record);

#endif
