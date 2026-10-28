#ifndef SOUND_COMMANDS_H
#define SOUND_COMMANDS_H

#include "common.h"

/* Command interface of modules/sound_control.c, adapting sound_driver.h.
 * master_volume.h publishes its frame service, transfer latch and ramps.
 * Shared driver, preset and queue state makes these non-reentrant. IDs,
 * preset indices and buffer sizes are unchecked; addresses retain the
 * original signed-word ABI. */

/* Play an effect on the inclusive SPU range 0..23 and return its voice.
 * The caller must ensure the driver can allocate a voice. Stop acts on the
 * returned voice, not on the effect ID. The final-voice APIs always use 23. */
s32 PlaySoundEffect(s16 soundId);
void StopSoundEffectVoice(s32 voice);
s32 GetLastSpuVoiceIndex(void);
s32 MuteLastSpuVoice(void);

/* Play track zero of a loaded sequence slot at equal left/right volume,
 * then select and enable a {type, depth} reverb preset. The preset is applied
 * even if the sequence was already playing. playParameter is saved by the
 * driver; it is not the SDK loop count (which is passed as zero).
 * Volume arrives as a word (800BC9AC passes a word accumulator) and is
 * explicitly narrowed to s16 here, before entering the sequence driver. */
s32 PlaySoundSequenceWithReverb(s16 slot, s16 playParameter,
                                s16 reverbPreset, s32 volume);
s32 StopSoundSequence(s16 slot);

/* Install the CD staging-buffer address for command 0; the first argument
 * is ignored. Its capacity must satisfy the driver's sector-rounded reads.
 * The dispatcher retains an unused second argument for its callers' ABI. */
void SetSoundCommandReadBuffer(s32 unused, s32 readBuffer);
/* Always clear the transfer-completion latch, then:
 *   0: if the reset guard is clear, silence keys, clear voice timestamps,
 *      configure the fixed destination area and queue bank 0;
 *   999 / 1000: queue bank 2 / 1;
 *   otherwise: queue the command table's bank/sequence pair, using bank
 *      slot 2 for the sequence. Negative/out-of-range indices are unsafe.
 * This only queues work; UpdateMasterVolumeAndSoundTransfer services it. */
void QueueSoundLoadCommand(s16 command, s16 unused);

#endif
