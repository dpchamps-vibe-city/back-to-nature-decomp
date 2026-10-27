#ifndef OVL11_CD_AUDIO_COMMAND_H
#define OVL11_CD_AUDIO_COMMAND_H

#include "common.h"

/* Object-command adapter for CD/XA playback. While the message window is
 * active, return 1 without issuing a command, even for a begin request.
 * Otherwise begin != 0 selects the signed low halfword of trackIndex and
 * returns 0; begin == 0 returns 1 only when CD audio is stopped.
 * This does not poll playback or stop a track. The normal CD audio service
 * must run separately. Track indices are unchecked. The two reserved command
 * operands are supplied by the dispatcher/direct scene caller but ignored.
 */
s32 Ovl11CdAudioCommand(s32 trackIndex, s32 reserved1, s32 begin,
                        s32 reserved3);

#endif
