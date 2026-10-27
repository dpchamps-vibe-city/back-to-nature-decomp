#ifndef OVL11_SOUND_EFFECT_COMMAND_H
#define OVL11_SOUND_EFFECT_COMMAND_H

#include "common.h"

/* ovl_11 command: PlaySoundEffect(soundId), discard its allocated voice and
 * return 1. Does not wait for playback. Sound driver/bank initialization and
 * a valid ID/available voice are required (sound_commands.h). Only the first
 * command operand is read. Original identity: ovl_11:800E95FC. */
s32 Ovl11PlaySoundEffectCommand(s16 soundId);

#endif
