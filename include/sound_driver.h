#ifndef SOUND_DRIVER_H
#define SOUND_DRIVER_H

#include "common.h"

/* Six bank/sequence slots and 24 SPU voices. All state is shared and calls
 * are non-reentrant. Addresses below retain the original signed-word ABI;
 * RAM and archive lengths are bytes, not sectors. Slot/ID/range arguments
 * are unchecked. The master-volume ramp is a separate interface. */
void InitSoundOutputAndCdFiles(void);
void InitSoundDriver(void);
void ResetSoundDriverSlots(void);
void ClearSoundVoiceKeyOnFrames(void);

/* Configure destination RAM and a CD staging buffer, reset all slot state,
 * and restart SPU allocation at 0x1010. Reconfiguration closes prior slots.
 * The final two arguments are retained ABI inputs but are not read. */
s32 ConfigureSoundLoadBuffers(s32 destination, s32 readBuffer,
                              s32 unused2, s32 unused3);
/* Queue up to six loads between resets; no capacity check is made.
 * IDs select pairs in gSoundArchiveOffsets. A sequence references a loaded
 * bank slot. Neither queue operation starts I/O. Both return zero. */
s32 QueueSoundBankLoad(s32 assetId);
s32 QueueSoundSequenceLoad(s32 assetId, s32 bankSlot);
/* Service through UpdateMasterVolumeAndSoundTransfer once per update.
 * GetPendingSoundLoad/StepSoundLoad return the current doubled asset ID,
 * or -1 at the sentinel. Rewind closes slots at/above the supplied cursors
 * and clears the request list, stopping the reclaimed sequences. */
s32 GetPendingSoundLoad(void);
s32 StepSoundLoad(void);
s32 RewindSoundLoadSlots(s32 bankSlot, s32 sequenceSlot);
s32 GetSoundSpuLoadAddress(void);
s32 GetSoundRamLoadCursor(void);
s32 OpenSoundSequencesAndStart(void);
s32 CloseSoundDriverSlots(void);

/* The SDK is configured for six sequences and one track per sequence.
 * trackOffset is added to slot in the state table (normally zero).
 * Play only acts on a stopped (-1) state, silences all keys, and loops the
 * sequence. playParameter is saved for the other sequence-control APIs. */
s32 PlaySoundSequenceSlot(s32 slot, s32 trackOffset, s32 left, s32 right,
                          s16 playParameter);
s32 SetSoundSequenceSlotVolume(s32 slot, s32 trackOffset, s32 unused2,
                               s32 volume, s32 unused4);
s32 StopSoundSequenceSlot(s32 slot, s32 trackOffset);
s32 KeyOffSoundSequenceVoices(s32 sequence, s32 track, s32 voiceMask);

/* Voice ranges are inclusive and must contain an idle voice or a voice in
 * one of groups 0..3; playback does not guard an allocator result of -1.
 * The idle scan rejects key-on stamps less than two unsigned ticks away;
 * fallback steals the oldest voice in the lowest available group.
 * Effects 0x46/0x47 randomize pitch and left/right volume. Returns voice. */
s32 PlaySoundEffectInVoiceRange(s32 soundId, s32 firstVoice, s32 lastVoice);
s32 KeyOffSoundVoice(s32 voice);
s32 MuteAndKeyOffSoundVoice(s16 voice);
s32 SetSoundVoiceVolume(s16 voice, s16 left, s16 right);
s32 SetSoundReservedVoiceCount(s32 count);

/* SDK reverb parameters and volumes are passed through in SDK units.
 * Selecting a type also clears reverb depth. Stereo/mono selection is
 * remembered for the next sequencer start. */
s32 SetSoundReverbType(s16 type);
void SetSoundReverbDepth(s16 depth);
void SetSoundReverbVolume(s16 volume);
void SetSoundReverbFeedback(s16 feedback);
void SetSoundReverbDelay(s16 delay);
void EnableSoundReverb(void);
void DisableSoundReverb(void);
void SetSoundMono(void);
void SetSoundStereo(void);
s32 GetSoundStereoEnabled(void);

#endif
