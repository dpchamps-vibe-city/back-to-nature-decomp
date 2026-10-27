/*
 * record_timed_sprite_command — reconstructed translation unit.
 *
 * Start or acknowledge completion of the timed sprite effect embedded in a
 * selected position record.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_record_timed_sprite_command.h"

s32 Ovl11PollRecordTimedSpriteCommand(s16 recordId, s16 animationId) {
    s16 *effect;

    if (recordId == 0x29) {
        effect = gObjectWorkTimedSpriteEffect;
    } else {
        effect = (s16 *)((char *)gPositionRecordTimedSpriteEffects + recordId * 0x1D4);
    }
    if (effect[0] != -1) {
        if (effect[1] == 0) {
            Ovl11ResetTimedSpriteEffect(effect);
            return 1;
        }
        return 0;
    }
    Ovl11StartTimedSpriteEffect(effect, animationId, 30);
    return 0;
}
