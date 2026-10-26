/* Debug flag/sound-test page and its complete helper chain. Row 8 swaps
 * sound banks on entry/exit; asynchronous loads return wait states to the
 * menu controller rather than blocking. Public contract:
 * ovl30_debug_flags_sound.h. Membership/boundaries: notes/file-groupings.md. */

#include "ovl30_debug_flags_sound.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "cd_audio.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"

void Ovl30EnterSoundTestRow(void) {
    RewindSoundLoadSlots(2, 0);
    RewindSoundLoadSlots(1, 0);
    QueueSoundLoadCommand(0x3E7, 0);
}

void Ovl30LeaveSoundTestRow(void) {
    RewindSoundLoadSlots(2, 0);
    RewindSoundLoadSlots(1, 0);
    QueueSoundLoadCommand(0x3E8, 0);
}

void Ovl30DebugFlagsPageHook(void) {
}

s32 Ovl30WriteDebugFlagBit(s32 bitIndex, s32 value);
void Ovl30StepDebugSetting(void);

#define DEBUG_FLAGS ((struct Ovl30DebugFlags *)D_8006C838)
#define INPUT ((SomeStruct *)gPad0Input)

s32 Ovl30UpdateDebugFlagsAndSound(s32 state) {
    s32 flags[15];
    u32 i;

    Ovl30DebugFlagsPageHook();
    for (i = 0; i < 15; i++) {
        flags[i] = 0;
    }
    if (DEBUG_FLAGS->flags10 < 0) flags[0] = 1;
    if (DEBUG_FLAGS->flags10 & 0x40000000) flags[1] = 1;
    if (!(DEBUG_FLAGS->flags10 & 0x20000000)) flags[2] = 1;
    if (DEBUG_FLAGS->flags10 & 0x10000000) flags[3] = 1;
    if (DEBUG_FLAGS->flags10 & 0x08000000) flags[4] = 1;
    if (DEBUG_FLAGS->flags10 & 0x04000000) flags[10] = 1;
    if (DEBUG_FLAGS->flagsC & 0x00800000) flags[11] = 1;
    if (DEBUG_FLAGS->flags10 & 0x02000000) flags[12] = 1;
    if (DEBUG_FLAGS->flags10 & 0x01000000) flags[13] = 1;
    if (DEBUG_FLAGS->flags10 & 0x00800000) flags[14] = 1;

    switch (state) {
    case 0:
        state = 1;
        gOvl30FlagRowIndex = 0;
        gOvl30SoundEffectIndex = 0;
        gOvl30AlternateSoundEffectIndex = 0;
        gOvl30SoundSequenceIndex = 0;
        gOvl30CdAudioTrackIndex = 0;
        gOvl30SoundTestVoice = -1;
        gOvl30DebugSettingValue = ((struct Ovl30SettingsView *)&D_8007AFF0)->selected;
        break;
    case 1:
        FntPrint(gOvl30FlagsSoundHeading);
        FntPrint(gOvl30ModelNumberFormat, gOvl30DebugSettingValue);
        FntPrint(gOvl30SoundEffectFormat, gOvl30SoundEffectIndex);
        FntPrint(gOvl30AlternateSoundEffectFormat, gOvl30AlternateSoundEffectIndex);
        FntPrint(gOvl30SoundSequenceFormat, gOvl30SoundSequenceIndex);
        FntPrint(gOvl30CdAudioTrackFormat, gOvl30CdAudioTrackIndex);
        if (INPUT->field_0x0 & 0x1000) {
            if (gOvl30FlagRowIndex == 8) {
                state = 1000;
                Ovl30LeaveSoundTestRow();
            }
            if (gOvl30FlagRowIndex == 0) {
                gOvl30FlagRowIndex = gOvl30FlagRowCount - 1;
            } else {
                gOvl30FlagRowIndex--;
            }
        } else if (INPUT->field_0x0 & 0x4000) {
            if (gOvl30FlagRowIndex == 8) {
                state = 1000;
                Ovl30LeaveSoundTestRow();
            }
            if (gOvl30FlagRowIndex == gOvl30FlagRowCount - 1) {
                gOvl30FlagRowIndex = 0;
            } else {
                gOvl30FlagRowIndex++;
            }
        } else {
            goto display_rows;
        }
        if (gOvl30FlagRowIndex == 8) {
            state = 1000;
            Ovl30EnterSoundTestRow();
        }
        break;
    display_rows:
        for (i = 0; i < gOvl30FlagRowCount; i++) {
            if (i == gOvl30FlagRowIndex) {
                FntPrint(gOvl30FlagCursorMarker);
            } else {
                FntPrint(gOvl30FlagIdleMarker);
            }
            FntPrint(gOvl30FlagRowFormat, gOvl30FlagRowLabels[i], gOvl30FlagStatusText[flags[i]]);
        }
        switch (gOvl30FlagRowIndex) {
        case 10:
            if (INPUT->field_0x8 & 0x2000) Ovl30WriteDebugFlagBit(26, 1);
            if (INPUT->field_0x8 & 0x8000) Ovl30WriteDebugFlagBit(26, 0);
            break;
        case 11:
            if (INPUT->field_0x8 & 0x2000) DEBUG_FLAGS->flagsC |= 0x800000;
            if (INPUT->field_0x8 & 0x8000) DEBUG_FLAGS->flagsC &= ~0x800000;
            break;
        case 12:
            if (INPUT->field_0x8 & 0x2000) DEBUG_FLAGS->flags10 |= 0x02000000;
            if (INPUT->field_0x8 & 0x8000) DEBUG_FLAGS->flags10 &= ~0x02000000;
            break;
        case 13:
            if (INPUT->field_0x8 & 0x2000) DEBUG_FLAGS->flags10 |= 0x01000000;
            if (INPUT->field_0x8 & 0x8000) DEBUG_FLAGS->flags10 &= ~0x01000000;
            break;
        case 14:
            if (INPUT->field_0x8 & 0x2000) DEBUG_FLAGS->flags10 |= 0x800000;
            if (INPUT->field_0x8 & 0x8000) DEBUG_FLAGS->flags10 &= ~0x800000;
            break;
        default:
            if (INPUT->field_0x8 & 0x2000) Ovl30WriteDebugFlagBit(31 - gOvl30FlagRowIndex, 1);
            if (INPUT->field_0x8 & 0x8000) Ovl30WriteDebugFlagBit(31 - gOvl30FlagRowIndex, 0);
            break;
        case 5:
            Ovl30StepDebugSetting();
            break;
        case 6:
            if (INPUT->field_0x8 & 0x20) {
                if (gOvl30SoundTestVoice != -1) StopSoundEffectVoice(gOvl30SoundTestVoice);
                gOvl30SoundTestVoice = PlaySoundEffect(gOvl30SoundEffectIndex);
            } else {
                if (INPUT->field_0x0 & 0x2000) {
                    if (++gOvl30SoundEffectIndex >= 77) gOvl30SoundEffectIndex = 0;
                }
                if (INPUT->field_0x0 & 0x8000) {
                    if (--gOvl30SoundEffectIndex < 0) gOvl30SoundEffectIndex = 76;
                }
            }
            break;
        case 7:
            if (INPUT->field_0x8 & 0x20) {
                state = 999;
                StopSoundSequence(0);
                OpenSoundSequencesAndStart();
                RewindSoundLoadSlots(2, 0);
                QueueSoundLoadCommand(gOvl30SoundSequenceIndex + 1, 0);
            } else {
                if (INPUT->field_0x0 & 0x2000) {
                    if (++gOvl30SoundSequenceIndex >= 37) gOvl30SoundSequenceIndex = 0;
                }
                if (INPUT->field_0x0 & 0x8000) {
                    if (--gOvl30SoundSequenceIndex < 0) gOvl30SoundSequenceIndex = 35;
                }
            }
            break;
        case 8:
            if (INPUT->field_0x8 & 0x20) {
                if (gOvl30SoundTestVoice != -1) StopSoundEffectVoice(gOvl30SoundTestVoice);
                gOvl30SoundTestVoice = PlaySoundEffect(gOvl30AlternateSoundEffectIndex + 54);
            } else {
                if (INPUT->field_0x0 & 0x2000) {
                    if (++gOvl30AlternateSoundEffectIndex >= 16) gOvl30AlternateSoundEffectIndex = 0;
                }
                if (INPUT->field_0x0 & 0x8000) {
                    if (--gOvl30AlternateSoundEffectIndex < 0) gOvl30AlternateSoundEffectIndex = 15;
                }
            }
            break;
        case 9:
            if (INPUT->field_0x8 & 0x20) {
                StartCdAudioTrack(gOvl30CdAudioTrackIndex);
            } else {
                if (INPUT->field_0x0 & 0x2000) {
                    if (++gOvl30CdAudioTrackIndex >= 26) gOvl30CdAudioTrackIndex = 0;
                }
                if (INPUT->field_0x0 & 0x8000) {
                    if (--gOvl30CdAudioTrackIndex < 0) gOvl30CdAudioTrackIndex = 25;
                }
            }
            break;
        }
        break;
    case 999:
        if (IsSoundTransferComplete()) {
            state = 1;
            OpenSoundSequencesAndStart();
            PlaySoundSequenceWithReverb(0, 0, gOvl30SoundSequenceIndex, 127);
        }
        break;
    case 1000:
        if (IsSoundTransferComplete()) {
            state = 1;
            OpenSoundSequencesAndStart();
        }
        break;
    }
    return state;
}

/* Keep the asymmetric endpoint behavior: decrementing zero yields 256,
 * whereas incrementing 255 yields zero. This is not an 8-bit wrap. */
void Ovl30StepDebugSetting(void) {
    if (((SomeStruct *)gPad0Input)->field_0x0 & 0x2000) {
        gOvl30DebugSettingValue += 1;
        if (gOvl30DebugSettingValue == 0x100) {
            gOvl30DebugSettingValue = 0;
        }
    }

    if (((SomeStruct *)gPad0Input)->field_0x0 & 0x8000) {
        if (gOvl30DebugSettingValue == 0) {
            gOvl30DebugSettingValue = 0x100;
            return;
        }
        gOvl30DebugSettingValue -= 1;
    }
}

/* Callers supply bitIndex 23..31 and value 0/1; return the clear mask.
 * Keep the unsigned expression view of the word the page reads as signed. */
s32 Ovl30WriteDebugFlagBit(s32 bitIndex, s32 value) {
    ((Ovl30UnsignedFlagWord *)&D_8006C838)->bits = (((Ovl30UnsignedFlagWord *)&D_8006C838)->bits & ~(1 << bitIndex)) | (value << bitIndex);
    return ~(1 << bitIndex);
}
