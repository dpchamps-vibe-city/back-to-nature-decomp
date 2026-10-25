/*
 * CD/XA playback: discover four files, select a track's file/channel, and
 * service its asynchronous read/position handshake and serial-volume decay.
 * cd_audio.h describes drive sharing, blocking behavior and state results.
 *
 * The five playback functions share the GP-relative state below. The file
 * discovery helper supplies their private location table. Original function
 * order is retained (exe:800218C4..80021D64); see notes/file-groupings.md.
 */
#include "common.h"
#include "cd_audio.h"
#include "psyq/stddef.h"
#include "psyq/libcd.h"
#include "psyq/libsnd.h"
#include "update_timing.h"

/* Tentative definitions preserve this unit's original small-data ownership. */
s16 gCdAudioState;
s16 gCdAudioVolumeCountdown;
s32 gCdAudioStartSector;
s32 gCdAudioLastSector;
s32 gCdAudioPollPhase;
s32 gCdAudioRestartCount;
s32 gCdAudioTrackIndex;
s32 gCdAudioFileIndex;
s32 gCdAudioChannel;

s32 func_8001AF44(u32 arg0);

void FindCdAudioFiles(void) {
    s32 fileIndex;
    s32 wordOffset;
    void *found;

    for (fileIndex = 0; fileIndex < 4; fileIndex++) {
        wordOffset = fileIndex * 3;
        do {
            found = CdSearchFile((CdlFILE *)(&gCdAudioFileLocationStorage[0] + (wordOffset << 1)),
                                 (char *)gCdAudioFileNames[fileIndex]);
        } while (found == 0);
    }
}

void PollCdAudio(void) {
    u8 response[8];
    s32 timingMode;
    s32 volumeStep;
    s32 nextVolume;
    s32 syncResult;
    s32 sector;

    if (gCdAudioState != 0) {
        timingMode = GetUpdateTimingMode();
        switch (timingMode) {
        case 0:
        case 1:
            volumeStep = 1;
            break;
        case 2:
            volumeStep = 2;
            break;
        case 3:
            volumeStep = 3;
            break;
        default:
            volumeStep = 1;
            break;
        }
        switch (gCdAudioPollPhase) {
        case 0:
        case 1:
        case 2:
            if (gCdAudioPollPhase >= 0) {
                gCdAudioPollPhase++;
            }
            break;
        case 3:
            if ((CdSync(1, response) == CdlComplete) &&
                (RestartCdAudioCommand(CdlReadS) == 0)) {
                gCdAudioPollPhase++;
            }
            break;
        case 4:
            syncResult = CdSync(1, response);
            if (syncResult == CdlDiskError) {
                RestartCdAudioCommand(CdlReadS);
                break;
            }
            if (syncResult == CdlComplete) {
                if (gCdAudioVolumeCountdown <= 0) {
                    StopCdAudio();
                } else if (gCdAudioLastSector < (gCdAudioStartSector - 0x12C)) {
                    RestartCdAudioCommand(CdlReadS);
                } else {
                    if (CdLastCom() == CdlGetlocP) {
                        sector = CdPosToInt((CdlLOC *)&response[5]);
                        if (sector > 0) {
                            gCdAudioLastSector = sector;
                        }
                    }
                    gCdAudioState = 2;
                    CdControlF(CdlGetlocP, 0);
                    nextVolume = gCdAudioVolumeCountdown - volumeStep;
                    if (nextVolume < 0) {
                        nextVolume = 0;
                    }
                    gCdAudioVolumeCountdown = nextVolume;
                    /* Preserve the discontinuity: above 32 use 45, but at
                     * 32 the level becomes 128 before falling in steps of 4. */
                    if (gCdAudioVolumeCountdown < 0x21) {
                        SsSetSerialVol(0, gCdAudioVolumeCountdown * 4,
                                      gCdAudioVolumeCountdown * 4);
                    } else {
                        SsSetSerialVol(0, 0x2D, 0x2D);
                    }
                }
            }
            break;
        }
    }
}

void StopCdAudio(void) {
    if (gCdAudioState != 0) {
        SsSetSerialVol(0, 0, 0);
        CdStandby();
        gCdAudioState = 0;
    }
}

s32 GetCdAudioState(void) {
    if (gCdAudioState == 0) {
        return 0;
    }
    if (gCdAudioState == 2) {
        return 2;
    }
    return 1;
}

void StartCdAudioTrack(s32 trackIndex) {
    struct struct_8006C838_view *gameState;
    CdlLOC mode;
    u8 filter[2];
    s32 sector;

    gameState = (struct struct_8006C838_view *)&D_8006C838;
    if ((gameState->field_CC == 0) &&
        ((func_8001AF44(2) != 1) || (*(s32 *)((u8 *)gameState + 0x7A74) == 1))) {
        /* {countdown, file index, XA channel}. Keep the halfword-base
         * expression: it preserves the original field-load scheduling. */
        u16 *settings = (u16 *)gCdAudioTrackSettings + trackIndex * 3;
        CdFlush();
        StopCdAudio();
        SsSetSerialVol(0, 0, 0);
        gCdAudioChannel = ((s16 *)settings)[2];
        gCdAudioFileIndex = ((s16 *)settings)[1];
        gCdAudioVolumeCountdown = settings[0];
        gCdAudioState = 1;
        gCdAudioPollPhase = 0;
        gCdAudioTrackIndex = trackIndex;
        filter[0] = 1;
        filter[1] = (u8)gCdAudioChannel;

        do { } while (CdControlB(CdlSetfilter, filter, 0) != 1);
        sector = CdPosToInt((CdlLOC *)((u8 *)gCdAudioFileLocationStorage +
                                      gCdAudioFileIndex * 24));
        gCdAudioStartSector = sector;
        gCdAudioLastSector = sector;
        gCdAudioRestartCount = 0;
        /* Only the first byte is sent for CdlSetmode. */
        mode.minute = 0xC8;
        do { } while (CdControlB(CdlSetmode, (u_char *)&mode, 0) != 1);
    }
}

s32 RestartCdAudioCommand(s32 command) {
    CdlLOC location;
    s32 settingsOffset;
    u16 countdown;

    command &= 0xFF;
    settingsOffset = gCdAudioTrackIndex * 6;
    countdown = *(u16 *)((u8 *)gCdAudioTrackSettings + settingsOffset);
    gCdAudioState = 1;
    gCdAudioVolumeCountdown = countdown;
    CdIntToPos(gCdAudioStartSector, &location);
    if (CdControl(command, (u_char *)&location, 0) != 1) {
        return -1;
    }
    gCdAudioLastSector = gCdAudioStartSector;
    gCdAudioRestartCount += 1;
    return 0;
}
