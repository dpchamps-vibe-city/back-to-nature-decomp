/*
 * sound_effect_command — reconstructed translation unit.
 *
 * Play a resident sound effect and report unconditional command success,
 * discarding the allocated voice.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "sound_commands.h"
#include "ovl11_sound_effect_command.h"

s32 Ovl11PlaySoundEffectCommand(s16 soundId) {
    PlaySoundEffect(soundId);
    return 1;
}
