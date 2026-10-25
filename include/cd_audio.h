#ifndef CD_AUDIO_H
#define CD_AUDIO_H

#include "common.h"

/* CD/XA playback shares the physical drive with cd_loader. Calls are not
 * reentrant; do not overlap archive reads and playback commands. */

/* Search the four configured STR/*.XA paths into 24-byte file records.
 * Retries each search indefinitely; call before starting a track. */
void FindCdAudioFiles(void);

/* Select an unchecked track-table index.
 * May do nothing when the game-state guards forbid playback. Otherwise mutes
 * serial audio, installs the XA filter and mode, and resets the polling state.
 * Blocking control commands retry indefinitely. Playback begins in the poll. */
void StartCdAudioTrack(s32 trackIndex);

/* Advance the asynchronous command handshake once. During active playback,
 * request the current sector and decrease the volume countdown by the update
 * timing divisor (1, 2 or 3); this is a count of eligible polls, not seconds.
 * At zero, stop playback. Requires the regular frame-loop service. */
void PollCdAudio(void);

/* Mute and put the drive in standby if active; then report state 0. */
void StopCdAudio(void);

/* 0 = stopped, 1 = starting/restarting, 2 = active position polling. */
s32 GetCdAudioState(void);

/* Reissue a low-byte CD command at the selected file's start sector, resetting
 * the volume countdown. Return 0 on command acceptance, -1 otherwise. State
 * and countdown change even on failure; the restart count changes on success.
 * The poller uses CdlReadS. Exposed for retained external linkage, not a
 * general-purpose CD command interface. */
s32 RestartCdAudioCommand(s32 command);

#endif
