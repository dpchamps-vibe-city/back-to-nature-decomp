#ifndef OVL11_RECORD_TIMED_SPRITE_COMMAND_H
#define OVL11_RECORD_TIMED_SPRITE_COMMAND_H

#include "common.h"

/* ovl_11 must be resident. recordId == 41 selects the primary object's
 * embedded effect; all other signed IDs index the 0x1D4-stride record pool
 * without validation. The effect consists of signed animation ID/ticks.
 *
 * An idle ID (-1) starts animationId for 30 ticks and returns 0. An active
 * effect with zero ticks is reset to idle and returns 1. Otherwise return 0.
 * This command neither draws nor decrements ticks: the record's update/draw
 * path must service the effect separately. Changing animationId while active
 * does not replace it. Polling again after completion starts a new effect.
 * Original identity: ovl_11:800E7C2C. */
s32 Ovl11PollRecordTimedSpriteCommand(s16 recordId, s16 animationId);

#endif
