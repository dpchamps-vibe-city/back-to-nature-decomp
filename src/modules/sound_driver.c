/*
 * sound_driver — reconstructed translation unit.
 *
 * Sequence-slot control, SPU voice allocation and staged sound-bank
 * transfers. Shared GP-owned request, sequence and transfer state joins
 * initialization through the final voice helpers; see sound_driver.h and
 * notes/file-groupings.md.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "sound_driver.h"
#include "cd_audio.h"
#include "psyq/libspu.h"
#include "psyq/memory.h"
#include "psyq/stddef.h"
#include "psyq/libetc.h"
#include "psyq/libsnd.h"
#include "psyq/libcd.h"
#include "include_asm.h"
#include "cd_loader.h"
#include "game_types.h"
#include "random.h"

void InitSoundOutputAndCdFiles(void) {
    SetSoundStereo();
    FindCdAudioFiles();
}

/* Tentative definitions — this TU owns these GP-relative globals. */
s32 gSoundLoadPhase;
s32 gSoundSequencerStarted;
s32 gSoundBufferConfigurationCount;
s32 gSoundStereoEnabled;

void InitSoundDriver(void) {
    s32 one;

    one = 1;
    SsInit();
    SsSetTableSize((char *)&D_8006C398, 6, 1);
    SsSetTickMode(1);
    SetSoundReservedVoiceCount(0x16);
    gSoundLoadPhase = 0;
    gSoundSequencerStarted = 0;
    gSoundBufferConfigurationCount = 0;
    gSoundStereoEnabled = one;
    SsUtReverbOff();
    D_8006C368.mask = 0x2C3;
    D_8006C368.mvol.left = 0x3FFF;
    D_8006C368.mvol.right = 0x3FFF;
    *(u16 *)((u16 *)&D_8006C368 + 8) = 0x1FFF;
    *(u16 *)((u16 *)&D_8006C368 + 9) = 0x1FFF;
    D_8006C368.cd.mix = one;
    SpuSetCommonAttr(&D_8006C368);
    SsSetSerialAttr(0, 0, 1);
    SsSetSerialVol(0, 0, 0);
    CdControl(9, 0, 0);
    ResetSoundDriverSlots();
}

void ClearSoundVoiceKeyOnFrames(void) {
    s32 i;

    i = 23;
    do {
        gSoundVoiceKeyOnFrames[i] = 0;
        i--;
    } while (i >= 0);
}

/* The [6][1] sequence tables retain a one-track inner loop. Reusing row
 * for the final voice loop also preserves the original loop optimization;
 * splitting it into an independent counter changes the generated code. */

s32 gSoundBankSlotCursor;
s32 gSoundLoadWriteIndex;
s32 gSoundLoadReadIndex;
s32 gSoundSequenceSlotCursor;

void ResetSoundDriverSlots(void) {
    s32 row;
    s32 col;
    s32 val;
    s32 *p1;
    s32 *p2;

    gSoundBankSlotCursor = 0;
    gSoundLoadReadIndex = 0;
    for (row = 0; row < 6; row++) {
        (&gSoundLoadIndices)[row] = -1;
        (&gSoundLoadIsSequence)[row] = 0;
        (&gSoundLoadBankSlots)[row] = 0;
        (&gSoundVabIds)[row] = -1;
        (&gSoundSequenceIds)[row] = -1;
        (&gSoundSequenceData)[row] = -1;
        (&gSoundBankRamStarts)[row] = -1;
        (&gSoundBankRamEnds)[row] = -1;
        (&gSoundSequenceVabIds)[row] = -1;
        (&gSoundSequenceByteSizes)[row] = -1;
        for (col = 0; col < 1; col++) {
            gSoundSequenceStates[row][col] = -1;
            gSoundSequencePlayParameters[row][col] = -1;
        }
    }
    val = 0x01000000;
    gSoundLoadWriteIndex = 0;
    gSoundSequenceSlotCursor = 0;
    p1 = &gSoundVoiceLastUseFrames;
    p2 = &gSoundVoiceGroups;
    for (row = 0; row < 24; row++) {
        *p1++ = val;
        *p2++ = 0;
    }
}

s32 gSoundSpuLoadAddress;
s32 gSoundRamLoadCursor;
s32 gSoundReadBuffer;

s32 ConfigureSoundLoadBuffers(s32 destination, s32 readBuffer, s32 unused2, s32 unused3) {
    if (gSoundBufferConfigurationCount) {
        CloseSoundDriverSlots();
    }
    gSoundRamLoadCursor = destination;
    gSoundReadBuffer = readBuffer;
    gSoundSpuLoadAddress = 0x1010;
    gSoundBufferConfigurationCount++;
    ResetSoundDriverSlots();
    return 0;
}

s32 QueueSoundBankLoad(s32 assetId) {
    (&gSoundLoadIndices)[gSoundLoadWriteIndex] = assetId * 2;
    gSoundLoadWriteIndex += 1;
    return 0;
}

s32 QueueSoundSequenceLoad(s32 assetId, s32 bankSlot) {
    s32 idx;

    idx = gSoundLoadWriteIndex;
    (&gSoundLoadIndices)[idx] = assetId * 2;
    (&gSoundLoadIsSequence)[idx] = 1;
    (&gSoundLoadBankSlots)[idx] = bankSlot;
    gSoundLoadWriteIndex = idx + 1;
    return 0;
}

s32 PlaySoundSequenceSlot(s32 slot, s32 trackOffset, s32 left, s32 right, s16 playParameter) {
    s32 *score_base;
    s32 *score_slot;

    score_base = &gSoundSequenceStates[0][0];
    trackOffset += slot;
    score_slot = &score_base[trackOffset];
    if (*score_slot == -1) {
        SsUtAllKeyOff(0);
        SsSeqSetVol((s16)(&gSoundSequenceIds)[slot], (s16)left, (s16)right);
        SsSeqStop((s16)(&gSoundSequenceIds)[slot]);
        SsSeqPlay((s16)(&gSoundSequenceIds)[slot], 1, 0);
        *score_slot = 0;
        (&gSoundSequencePlayParameters[0][0])[trackOffset] = playParameter;
    }
    return 0;
}

s32 SetSoundSequenceSlotVolume(s32 slot, s32 trackOffset, s32 unused2, s32 volume, s32 unused4) {
    if (gSoundSequenceStates[trackOffset + slot][0] == 0) {
        SsSeqSetVol((s16)(&gSoundSequenceIds)[slot], (s16)volume, (s16)volume);
    }
    return 0;
}

/* An active sequence stops when its requested volume is nonpositive. */
s32 func_800202F8(s32 slot, s32 trackOffset, s32 volume) {
    s32 *states;
    s32 *state;
    s32 stateIndex;

    states = &gSoundSequenceStates[0][0];
    stateIndex = trackOffset + slot;
    state = &states[stateIndex];
    if (*state != -1) {
        if (volume > 0) {
            SsSeqSetVol((s16)(&gSoundSequenceIds)[slot], (s16)volume, (s16)volume);
            return 1;
        } else {
            SsSeqStop((s16)(&gSoundSequenceIds)[slot]);
            *state = -1;
            return 0;
        }
    }
    return 0;
}

/* The upper volume bound is exclusive; there is no lower-bound check. */
s32 func_8002039C(s32 slot, s32 trackOffset, s32 volume) {
    if (gSoundSequenceStates[trackOffset + slot][0] != -1 && volume < 126) {
        SsSeqSetVol((s16)(&gSoundSequenceIds)[slot], (s16)volume, (s16)volume);
        return 1;
    }
    return 0;
}

/* Stub function - returns 0 */
s32 func_8002040C(void) {
    return 0;
}

/* Stop playback without closing the sequence handle; -1 permits replay. */

s32 StopSoundSequenceSlot(s32 slot, s32 trackOffset) {
    s32 *state;
    s32 *states;
    s32 stateIndex;

    states = &gSoundSequenceStates[0][0];
    stateIndex = trackOffset + slot;
    state = &states[stateIndex];
    if (*state != -1) {
        SsSeqStop((s16)(&gSoundSequenceIds)[slot]);
        *state = -1;
    }
    return 0;
}

/* Pause only a playing sequence: state 0 becomes paused state 1. */
s32 func_8002047C(s32 slot, s32 trackOffset) {
    s32 *states;
    s32 *state;
    s32 stateIndex;

    states = &gSoundSequenceStates[0][0];
    stateIndex = trackOffset + slot;
    state = &states[stateIndex];
    if (*state == 0) {
        SsSeqPause((s16)(&gSoundSequenceIds)[slot]);
        *state = 1;
    }
    return 0;
}

/* Resume only a paused sequence: state 1 becomes playing state 0. */
s32 func_800204E0(s32 slot, s32 trackOffset) {
    s32 *states;
    s32 *state;
    s32 stateIndex;

    states = &gSoundSequenceStates[0][0];
    stateIndex = trackOffset + slot;
    state = &states[stateIndex];
    if (*state == 1) {
        SsSeqReplay((s16)(&gSoundSequenceIds)[slot]);
        *state = 0;
    }
    return 0;
}

s32 SetSoundReservedVoiceCount(s32 count) {
    s32 result;

    result = SsSetReservedVoice(count & 0xFF);
    if ((result << 24) < 0) {
        return -1;
    }
    return 0;
}

INCLUDE_ASM("build/asm/nonmatchings/modules/sound_driver", func_80020570);

INCLUDE_ASM("build/asm/nonmatchings/modules/sound_driver", func_8002066C);

s32 KeyOffSoundSequenceVoices(s32 sequence, s32 track, s32 voiceMask) {
    SsVoKeyOff(track + (sequence << 8), voiceMask);
    return 0;
}

s32 MuteAndKeyOffSoundVoice(s16 voice) {
    s32 temp;

    SsUtSetVVol(voice, 0, 0);
    temp = SsUtKeyOffV(voice);
    if (temp < 0) {
        return -1;
    }
    return 0;
}

/* Stub function - returns 0 */
s32 func_800207DC(void) {
    return 0;
}

s32 SetSoundVoiceVolume(s16 voice, s16 left, s16 right) {
    SsUtSetVVol(voice, left, right);
    return 0;
}

s32 OpenSoundSequencesAndStart(void) {
    s32 *sequenceId;
    s32 *sequenceData;
    s32 *vabId;
    s32 data;
    s32 remaining;
    s32 empty;

    empty = -1;
    vabId = &gSoundSequenceVabIds;
    sequenceId = &gSoundSequenceIds;
    sequenceData = &gSoundSequenceData;
    remaining = 5;
    do {
        data = *sequenceData;
        if ((data != empty) && (*sequenceId == empty)) {
            *sequenceId = SsSeqOpen((unsigned long *)data, (short)*vabId);
        }
        vabId++;
        sequenceId++;
        remaining -= 1;
        sequenceData++;
    } while (remaining >= 0);
    if (gSoundSequencerStarted == 0) {
        SsStart();
    }
    gSoundSequencerStarted = 1;
    if (gSoundStereoEnabled != 0) {
        SsSetStereo();
    } else {
        SsSetMono();
    }
    return 0;
}

s32 SetSoundReverbType(s16 type) {
    s32 result;
    result = SsUtSetReverbType(type);
    SsUtSetReverbDepth(0, 0);
    return result;
}

void SetSoundReverbDepth(s16 depth) {
    SsUtSetReverbDepth(depth, depth);
}

void SetSoundReverbVolume(s16 volume) {
    SsSetRVol(volume, volume);
}

void SetSoundReverbFeedback(s16 feedback) {
    SsUtSetReverbFeedback(feedback);
}

void SetSoundReverbDelay(s16 delay) {
    SsUtSetReverbDelay(delay);
}

void EnableSoundReverb(void) {
    SsUtReverbOn();
}

void DisableSoundReverb(void) {
    SsUtReverbOff();
}

/* Tentative definition — this TU owns this GP-relative global. */

void SoundOutputModeChangedHook(void);

void SetSoundMono(void) {
    SsSetMono();
    gSoundStereoEnabled = 0;
    SoundOutputModeChangedHook();
}

/* Tentative definitions — this TU owns these GP-relative globals. */

void SetSoundStereo(void) {
    SsSetStereo();
    gSoundStereoEnabled = 1;
    SoundOutputModeChangedHook();
}

s32 GetSoundStereoEnabled(void) {
    return gSoundStereoEnabled;
}

s32 GetSoundSpuLoadAddress(void) {
    return gSoundSpuLoadAddress;
}

s32 GetSoundRamLoadCursor(void) {
    return gSoundRamLoadCursor;
}

s32 CloseSoundDriverSlots(void) {
    s32 i;
    s32 j;
    s32 *ptr;
    s32 sentinel;

    SsSetMVol(0, 0);

    for (i = 0; i < 6; i++) {
        j = 0;
        do {
            StopSoundSequenceSlot(i, j);
            j++;
        } while (j <= 0);
    }

    sentinel = -1;
    ptr = &gSoundSequenceIds;
    for (i = 0; i < 6; i++) {
        if (*ptr != sentinel) {
            SsSeqClose((s16) *ptr);
            *ptr = sentinel;
        }
        ptr++;
    }

    SsEnd();
    gSoundSequencerStarted = 0;

    ptr = &gSoundVabIds;
    for (i = 0; i < 6; i++) {
        if (*ptr >= 0) {
            SsVabClose((s16) *ptr);
            *ptr = -1;
        }
        ptr++;
    }

    return 0;
}

/* Reclaim the suffix of both slot tables and rewind the RAM/SPU cursors.
 * The volume call deliberately uses the slot number, not its SEQ handle. */
s32 RewindSoundLoadSlots(s32 arg0, s32 arg1) {
    s32 cur;
    s32 cand;
    s32 song;
    s32 col;

    cur = (&gSoundBankRamStarts)[arg0];
    gSoundBankSlotCursor = arg0;
    gSoundSequenceSlotCursor = arg1;
    if (cur != -1) {
        gSoundSpuLoadAddress = (&gSoundBankSpuAddresses)[arg0];
        cand = (&gSoundSequenceData)[arg1];
        if (((u32)cand < (u32)cur) && cand != -1) {
            gSoundRamLoadCursor = cand;
        } else {
            gSoundRamLoadCursor = cur;
        }
    }
    song = arg1;
    if (song < 6) {
        do {
            col = 0;
            do {
                SsSeqSetVol((s16)song, 0, 0);
                StopSoundSequenceSlot(song, col);
                col++;
            } while (col <= 0);
            song++;
        } while (song < 6);
    }
    for (song = arg1; song < 6; song++) {
        if ((&gSoundSequenceIds)[song] != -1) {
            SsSeqClose((s16)(&gSoundSequenceIds)[song]);
            (&gSoundSequenceIds)[song] = -1;
        }
        (&gSoundSequenceData)[song] = -1;
        (&gSoundSequenceVabIds)[song] = -1;
        (&gSoundSequenceByteSizes)[song] = -1;
    }
    for (song = arg0; song < 6; song++) {
        if ((&gSoundVabIds)[song] >= 0) {
            SsVabClose((s16)(&gSoundVabIds)[song]);
            (&gSoundVabIds)[song] = -1;
        }
        (&gSoundBankRamStarts)[song] = -1;
        (&gSoundBankRamEnds)[song] = -1;
        for (col = 0; col < 1; col++) {
            gSoundSequenceStates[song][col] = -1;
            gSoundSequencePlayParameters[song][col] = -1;
        }
    }
    gSoundSequenceSlotCursor = arg1;
    gSoundLoadReadIndex = 0;
    gSoundBankSlotCursor = arg0;
    for (song = 0; song < 6; song++) {
        (&gSoundLoadIndices)[song] = -1;
        (&gSoundLoadIsSequence)[song] = 0;
        (&gSoundLoadBankSlots)[song] = 0;
    }
    gSoundLoadWriteIndex = 0;
    return 0;
}

s32 GetPendingSoundLoad(void) {
    s32 *base;
    s32 index;
    
    base = &gSoundLoadIndices;
    index = gSoundLoadReadIndex;
    return base[index];
}

/* Tentative definitions — this TU owns these GP-relative globals
 * (every one of them is accessed via %gp_rel in the target). */
s32 gSoundLastSequenceBytes;

/* The loader progresses by phase, not by waiting for the whole bank.
 * 0 starts a CD read; 10 opens its VAB header; 20 completes its body;
 * 30/50 poll bank/sequence reads; 40 copies sequence data; 60 steps the
 * chunked VAB body. Other values leave the pending request unchanged. */
void StartSoundArchiveRead(s16);
void ConfigureSoundBankTransfer(s32, s32, s32);
void BeginSoundBankBodyTransfer(s32);
s32 StepSoundBankBodyTransfer(void);

s32 StepSoundLoad(void) {
    s32 value;

    switch (gSoundLoadPhase) {
    case 0x28:
    {
        s32 len;

        len = gSoundArchiveOffsets[(&gSoundLoadIndices)[gSoundLoadReadIndex] + 2] -
              gSoundArchiveOffsets[(&gSoundLoadIndices)[gSoundLoadReadIndex]];

        (&gSoundSequenceByteSizes)[gSoundSequenceSlotCursor] =
            gSoundArchiveOffsets[(&gSoundLoadIndices)[gSoundLoadReadIndex] + 1];
        gSoundLastSequenceBytes = len;
        memcpy((void *)gSoundRamLoadCursor, (void *)gSoundReadBuffer, len);
        gSoundLoadPhase = 0;
        {
            s32 *c028 = &gSoundSequenceData;
            s32 *c068 = &gSoundSequenceVabIds;
            s32 *ba8 = &gSoundVabIds;

            c028[gSoundSequenceSlotCursor] = gSoundRamLoadCursor;
            c068[gSoundSequenceSlotCursor] = ba8[(&gSoundLoadBankSlots)[gSoundLoadReadIndex]];
        }
        gSoundRamLoadCursor += gSoundArchiveOffsets[(&gSoundLoadIndices)[gSoundLoadReadIndex] + 2] -
                      gSoundArchiveOffsets[(&gSoundLoadIndices)[gSoundLoadReadIndex]];
        gSoundSequenceSlotCursor += 1;
        gSoundLoadReadIndex += 1;
        break;
    }
    case 0xA:
    {
        s32 head;

        memcpy((void *)gSoundRamLoadCursor, (void *)gSoundReadBuffer,
               gSoundArchiveOffsets[(&gSoundLoadIndices)[gSoundLoadReadIndex] + 1] -
                   gSoundArchiveOffsets[(&gSoundLoadIndices)[gSoundLoadReadIndex]]);
        (&gSoundBankSpuAddresses)[gSoundBankSlotCursor] = gSoundSpuLoadAddress;
        head = SsVabOpenHead(gSoundRamLoadCursor, -1);
        (&gSoundVabIds)[gSoundBankSlotCursor] = head;
        BeginSoundBankBodyTransfer(head);
        gSoundLoadPhase = 0x3C;
        break;
    }
    case 0x14:
        SsVabTransCompleted(1);
        gSoundLoadPhase = 0;
        (&gSoundBankRamStarts)[gSoundBankSlotCursor] = gSoundRamLoadCursor;
        gSoundRamLoadCursor += gSoundArchiveOffsets[(&gSoundLoadIndices)[gSoundLoadReadIndex] + 1] -
                      gSoundArchiveOffsets[(&gSoundLoadIndices)[gSoundLoadReadIndex]];
        (&gSoundBankRamEnds)[gSoundBankSlotCursor] = gSoundRamLoadCursor;
        gSoundBankSlotCursor += 1;
        gSoundLoadReadIndex += 1;
        break;
    case 0x0:
        StartSoundArchiveRead(*(s16 *)(&gSoundLoadIndices + gSoundLoadReadIndex));
        break;
    case 0x1E:
        value = (&gSoundLoadIndices)[gSoundLoadReadIndex];

        if (CdReadArchiveSectors(9, gSoundArchiveOffsets[value],
                          gSoundArchiveOffsets[value + 1] - gSoundArchiveOffsets[value],
                          gSoundReadBuffer, 0) == 1) {
            gSoundLoadPhase = 0xA;
            ConfigureSoundBankTransfer(value, gSoundReadBuffer, 0x2D000);
        }
        break;
    case 0x32:
        value = (&gSoundLoadIndices)[gSoundLoadReadIndex];

        if (CdReadArchiveSectors(9, gSoundArchiveOffsets[value],
                          gSoundArchiveOffsets[value + 2] - gSoundArchiveOffsets[value],
                          gSoundReadBuffer, 0) == 1) {
            gSoundLoadPhase = 0x28;
        }
        break;
    case 0x3C:
        if (StepSoundBankBodyTransfer() != 0) {
            gSoundLoadPhase = 0x14;
            gSoundSpuLoadAddress += gSoundArchiveOffsets[(&gSoundLoadIndices)[gSoundLoadReadIndex] + 2] -
                          gSoundArchiveOffsets[(&gSoundLoadIndices)[gSoundLoadReadIndex] + 1];
        }
        break;
    default:
        break;
    }
    return (&gSoundLoadIndices)[gSoundLoadReadIndex];
}

/* Keep the first VSync sample for the key-on guard and the second for
 * voice age: a frame boundary can fall between them. */

s32 FindIdleSoundVoice(s32 arg0, s32 arg1, s32 arg2, u8 *arg3);

s32 FindOldestSoundVoiceByGroup(s32 start, s32 end);

s32 PlaySoundEffectInVoiceRange(s32 soundId, s32 lo, s32 hi) {
    char status[24];
    s32 vsync;
    s32 voice;
    s32 frame;
    s16 *base;
    s16 group;
    s16 vabId;
    s16 prog;
    s16 tone;
    s16 note;
    s16 fine;
    s16 voll;
    s16 volr;

    vsync = VSync(-1);
    SpuGetAllKeysStatus(status);
    voice = FindIdleSoundVoice(vsync, lo, hi, status);
    if (voice < 0) {
        voice = FindOldestSoundVoiceByGroup(lo, hi);
    }
    base = gSoundEffectToneParameters + soundId * 7;
    group = base[1];
    (&gSoundVoiceGroups)[voice] = group;
    frame = VSync(-1);
    (&gSoundVoiceLastUseFrames)[voice] = frame;
    vabId = base[0];
    prog = base[2];
    tone = base[3];
    note = base[4];
    fine = base[5];
    voll = base[6];
    volr = base[6];
    if (soundId == 0x46) {
        note = (s16)(Rand(4) + 0x3C);
        voll = (s16)(Rand(0x1C) + 0x64);
        volr = (s16)(Rand(0x1C) + 0x64);
    }
    if (soundId == 0x47) {
        note = (s16)(Rand(9) + 0x38);
        voll = (s16)(Rand(0x1C) + 0x64);
        volr = (s16)(Rand(0x1C) + 0x64);
    }
    SsUtKeyOnV((s16)voice, vabId, prog, tone, note, fine, voll, volr);
    gSoundVoiceKeyOnFrames[voice] = vsync;
    return voice;
}

s32 KeyOffSoundVoice(s32 arg0) {
    char status[24];

    SpuGetAllKeysStatus(status);

    if (status[arg0] == 0 || status[arg0] == 3) {
        return 0;
    }

    (&gSoundVoiceGroups)[arg0] = 0;
    (&gSoundVoiceLastUseFrames)[arg0] = 0x1000000;
    SsUtKeyOffV(arg0);

    return 0;
}

/* Tentative definitions — this TU owns these GP-relative globals. */
s16 gSoundLoadArchiveIndex;

void StartSoundArchiveRead(s16 arg0) {
    s32 temp;

    gSoundLoadArchiveIndex = arg0;
    if ((&gSoundLoadIndices)[gSoundLoadReadIndex] >= 0) {
        if ((&gSoundLoadIsSequence)[gSoundLoadReadIndex] != 0) {
            temp = gSoundArchiveOffsets[arg0];
            CdReadArchiveSectors(9, temp, gSoundArchiveOffsets[arg0 + 2] - temp, gSoundReadBuffer, 1);
            gSoundLoadPhase = 0x32;
            return;
        }
        temp = gSoundArchiveOffsets[arg0];
        CdReadArchiveSectors(9, temp, gSoundArchiveOffsets[arg0 + 1] - temp, gSoundReadBuffer, 1);
        gSoundLoadPhase = 0x1E;
        return;
    }
    gSoundLoadPhase = 0;
}

void ConfigureSoundBankTransfer(s32 archiveIndex, s32 readBuffer, s32 chunkBytes) {
    SoundBankTransferState *transfer = &gSoundBankTransfer;
    transfer->archiveIndex = archiveIndex;
    transfer->readBuffer = readBuffer;
    transfer->chunkBytes = chunkBytes;
}

void BeginSoundBankBodyTransfer(s32 vabId) {
    s32 index;
    s32 bodyEnd;
    s32 bodyStart;
    u32 bodyBytes;
    u32 lastChunk;

    index = gSoundBankTransfer.archiveIndex;
    bodyEnd = gSoundArchiveOffsets[index + 2];
    bodyStart = gSoundArchiveOffsets[index + 1];

    bodyBytes = (u32)(bodyEnd - bodyStart);
    lastChunk = bodyBytes / 184320U;

    gSoundBankTransfer.vabId = vabId;
    gSoundBankTransfer.readPending = 0;
    gSoundBankTransfer.chunkIndex = 0;
    gSoundBankTransfer.archiveByte = bodyStart;
    gSoundBankTransfer.lastChunk = lastChunk;
}

/* Each completed CD read submits a full chunk, including the last one.
 * The completion poll blocks here; the CD start/poll pair does not. */
s32 StepSoundBankBodyTransfer(void) {
    s32 readPending;

    if ((u32)gSoundBankTransfer.chunkIndex <= (u32)gSoundBankTransfer.lastChunk) {
        switch (gSoundBankTransfer.readPending) {
        case 0:
            CdReadArchiveSectors(9, gSoundBankTransfer.archiveByte, gSoundBankTransfer.chunkBytes, gSoundBankTransfer.readBuffer, 1);
            gSoundBankTransfer.readPending = 1;
            break;
        case 1:
            if (CdReadArchiveSectors(9, gSoundBankTransfer.archiveByte, gSoundBankTransfer.chunkBytes, gSoundBankTransfer.readBuffer, 0) == 1) {
                SsVabTransBodyPartly((u8 *)gSoundBankTransfer.readBuffer, gSoundBankTransfer.chunkBytes, gSoundBankTransfer.vabId);
                readPending = gSoundBankTransfer.readPending;
                gSoundBankTransfer.readPending = 0;
                gSoundPreviousBankTransfer.archiveIndex = gSoundBankTransfer.archiveIndex;
                gSoundPreviousBankTransfer.readBuffer = gSoundBankTransfer.readBuffer;
                gSoundPreviousBankTransfer.chunkBytes = gSoundBankTransfer.chunkBytes;
                gSoundPreviousBankTransfer.lastChunk = gSoundBankTransfer.lastChunk;
                gSoundPreviousBankTransfer.chunkIndex = gSoundBankTransfer.chunkIndex;
                gSoundPreviousBankTransfer.archiveByte = gSoundBankTransfer.archiveByte;
                gSoundPreviousBankTransfer.vabId = gSoundBankTransfer.vabId;
                gSoundPreviousBankTransfer.readPending = readPending;
                while (!SsVabTransCompleted(1)) {
                }
                gSoundBankTransfer.archiveByte = gSoundBankTransfer.archiveByte + gSoundBankTransfer.chunkBytes;
                gSoundBankTransfer.chunkIndex = gSoundBankTransfer.chunkIndex + 1;
            }
            break;
        }
        return 0;
    }
    return 1;
}

s32 FindIdleSoundVoice(s32 arg0, s32 arg1, s32 arg2, u8 *arg3) {
    s32 *v1;
    s32 three;
    u8 temp;
    s32 *base;

    three = 3;
    if (arg2 < arg1) {
        goto done;
    }
    base = &gSoundVoiceKeyOnFrames[0];
    v1 = (s32 *)((arg1 << 2) + (s32)base);
loop:
    temp = arg3[arg1];
    if ((temp == 0) || (temp == three)) {
        if ((u32)(*v1 - arg0) >= 2U) {
            return arg1;
        }
    }
    arg1 += 1;
    v1++;
    if (arg2 >= arg1) {
        goto loop;
    }
done:
    return -1;
}

/* Group priority precedes age: stop at the first group with an eligible
 * voice, then select its oldest stamp. The free stamp is the strict upper
 * bound, not a selectable timestamp. */
s32 FindOldestSoundVoiceByGroup(s32 start, s32 end) {
    s32 best;
    s32 pass;
    s32 i;
    s32 min;

    best = -1;
    for (pass = 0; pass < 4; pass++) {
        min = 0x1000000;
        for (i = start; i <= end; i++) {
            if ((&gSoundVoiceGroups)[i] == pass && (&gSoundVoiceLastUseFrames)[i] < min) {
                best = i;
                min = (&gSoundVoiceLastUseFrames)[i];
            }
        }
        if (best != -1) {
            return best;
        }
    }
    return -1;
}

void SoundOutputModeChangedHook(void) {
}
