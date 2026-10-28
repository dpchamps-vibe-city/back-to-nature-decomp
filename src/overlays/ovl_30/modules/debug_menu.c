/* Debug-menu control, staged setting selection and the flags/sound page.
 *
 * The first page initializes and edits gOvl30DebugSettingValue; the controller
 * publishes a changed value to gDebugMenuRequestedSelection only on exit,
 * with game flag 0x9C as the consumer's pending-change handshake. Other flags
 * and sound commands take effect immediately. Page state is passed back to
 * the controller, including asynchronous sound-transfer wait states.
 *
 * Display preservation uses heap+0x4290 until exit restoration. The display
 * helper is genuinely nested: its scope preserves the original static-chain
 * spill and code order. Do not turn it into an unrelated top-level helper.
 * Public entry, page and font contracts remain in the three ovl30 headers.
 * Membership and conservative outer boundaries: notes/file-groupings.md. */

#include "common.h"
#include "ovl30_debug_flags_sound.h"
#include "game_types.h"
#include "ovl30_debug_menu_lifecycle.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"
#include "scene_selection.h"
#include "cd_audio.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "game_flags.h"
#include "ovl30_debug_font.h"

void Ovl30ResetDebugMenu(void) {
    gOvl30DebugMenuPendingChange = 0;
    gOvl30DebugMenuState = 0;
}

s32 Ovl30GetDebugMenuState(void) {
    s32 state;

    state = gOvl30DebugMenuState;
    if (gOvl30DebugMenuState == 0) {
        gOvl30DebugMenuState = 0;
    }
    return state;
}

/* Retained original entries; no stronger purpose is established. */
s32 Ovl30DebugMenuReturnZero(void) {
    return 0;
}

void Ovl30DebugMenuNoOp(void) {
}

void Ovl30TransferDebugDisplay(s32 restore);

/* Keep the increment as an update before the upper clamp. The decrement
 * path uses a separate nonnegative clamp rather than unsigned wrapping. */
#define clamp_selection(expression) \
    ({ s32 value = (expression); \
       if ((u32)value >= 18) value = 17; \
       value; })

static inline s32 clamp_nonnegative(s32 value) {
    if (value < 0) {
        value = 0;
    }
    return value;
}

void Ovl30UpdateDebugMenu(void) {
    s32 (*pageHandler)();
    s32 fadeComplete;
    s32 buttons;
    s32 previousPage;
    s32 state = gOvl30DebugMenuState;
    s32 *index;

    auto void copy_display(s32 restore) __asm__("Ovl30TransferDebugDisplay");

    switch (state) {
    case 0:
        gOvl30DebugMenuState = 2;
        gOvl30DebugMenuWaitingForExit = 1;
        gOvl30DebugPageIndex = 0;
        gOvl30DebugPageState = 0;
        gOvl30ActiveDebugMenuHandler = gOvl30DebugMenuHandlers[0];
        D_80133FBC = 0;
        copy_display(0);
        break;
    case 2:
        gOvl30DebugMenuState = 3;
        KanjiFntClose();
        KanjiFntOpen(8, 0x10, 0x140, 0xF0, 0x1C0, 0, 0x140, 0xFF, 0, 0xFF);
        FntFlush(-1);
        Ovl30InitDebugFont();
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        break;
    case 3:
        buttons = gPad0Input->pressedButtons;
        if (buttons & 2) {
            index = &gOvl30DebugPageIndex;
            gOvl30DebugPageState = 0;
            gOvl30RecordMenuSelectedId = -1;
            *index = clamp_selection(*index += 1);
            PlaySoundEffect(4);
        } else if (buttons & 1) {
            previousPage = --gOvl30DebugPageIndex;
            gOvl30DebugPageState = 0;
            gOvl30RecordMenuSelectedId = -1;
            gOvl30DebugPageIndex = clamp_nonnegative(previousPage);
            PlaySoundEffect(4);
        }
        pageHandler = gOvl30DebugMenuHandlers[gOvl30DebugPageIndex];
        gOvl30ActiveDebugMenuHandler = pageHandler;
        gOvl30DebugPageState = pageHandler(gOvl30DebugPageState);
        fadeComplete = IsScreenFadeComplete();
        if (fadeComplete == 1) {
            if (gOvl30DebugMenuWaitingForExit == fadeComplete) {
                if (gPad0Input->pressedButtons & 0x800) {
                    BeginScreenFadeIncrease(0xA, 0, 2);
                    BeginMasterVolumeFadeOut(0xA);
                    gOvl30DebugMenuWaitingForExit = 0;
                }
            } else {
                gOvl30DebugMenuState = 4;
            }
        }
        KanjiFntFlush(0);
        break;
    case 4: {
        auto void copy_display(s32 restore) __asm__("Ovl30TransferDebugDisplay");
        void copy_display(s32 restore) {
            u_long *pixels;

            pixels = (u_long *)(gGraphicsHeapBase + 0x4290);
            if (restore == 0) {
                StoreImage((RECT *)gOvl30DebugDisplayRect, pixels);
            } else {
                LoadImage((RECT *)gOvl30DebugDisplayRect, pixels);
            }
            DrawSync(0);
        }
        copy_display(1);
        SetGameFlag(0x9CU, 0U);
        index = &gOvl30DebugSettingValue;
        if (*index != ((struct Ovl30SettingsView *)&D_8007AFF0)->selected) {
            SetGameFlag(0x9CU, 1U);
            gDebugMenuRequestedSelection = *index;
        }
        gOvl30DebugMenuState = 0;
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        ((struct struct_8006C838_view *)D_8006C838)->field_0C &= 0xF7FFFFFF;
        RequestSceneChange(2);
        SetGameFlag(0x9BU, 0U);
        if (gOvl30DebugMenuPendingChange != 0) {
            SetGameFlag(0x9BU, 1U);
        }
        break;
    }
    }
    PollCdAudio();
}

/* Debug flag/sound-test page and its complete helper chain. Row 8 swaps
 * sound banks on entry/exit; asynchronous loads return wait states to the
 * menu controller rather than blocking. Public contract:
 * ovl30_debug_flags_sound.h. Membership/boundaries: notes/file-groupings.md. */

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
        if (gPad0Input->repeatButtons & 0x1000) {
            if (gOvl30FlagRowIndex == 8) {
                state = 1000;
                Ovl30LeaveSoundTestRow();
            }
            if (gOvl30FlagRowIndex == 0) {
                gOvl30FlagRowIndex = gOvl30FlagRowCount - 1;
            } else {
                gOvl30FlagRowIndex--;
            }
        } else if (gPad0Input->repeatButtons & 0x4000) {
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
            if (gPad0Input->pressedButtons & 0x2000) Ovl30WriteDebugFlagBit(26, 1);
            if (gPad0Input->pressedButtons & 0x8000) Ovl30WriteDebugFlagBit(26, 0);
            break;
        case 11:
            if (gPad0Input->pressedButtons & 0x2000) DEBUG_FLAGS->flagsC |= 0x800000;
            if (gPad0Input->pressedButtons & 0x8000) DEBUG_FLAGS->flagsC &= ~0x800000;
            break;
        case 12:
            if (gPad0Input->pressedButtons & 0x2000) DEBUG_FLAGS->flags10 |= 0x02000000;
            if (gPad0Input->pressedButtons & 0x8000) DEBUG_FLAGS->flags10 &= ~0x02000000;
            break;
        case 13:
            if (gPad0Input->pressedButtons & 0x2000) DEBUG_FLAGS->flags10 |= 0x01000000;
            if (gPad0Input->pressedButtons & 0x8000) DEBUG_FLAGS->flags10 &= ~0x01000000;
            break;
        case 14:
            if (gPad0Input->pressedButtons & 0x2000) DEBUG_FLAGS->flags10 |= 0x800000;
            if (gPad0Input->pressedButtons & 0x8000) DEBUG_FLAGS->flags10 &= ~0x800000;
            break;
        default:
            if (gPad0Input->pressedButtons & 0x2000) Ovl30WriteDebugFlagBit(31 - gOvl30FlagRowIndex, 1);
            if (gPad0Input->pressedButtons & 0x8000) Ovl30WriteDebugFlagBit(31 - gOvl30FlagRowIndex, 0);
            break;
        case 5:
            Ovl30StepDebugSetting();
            break;
        case 6:
            if (gPad0Input->pressedButtons & 0x20) {
                if (gOvl30SoundTestVoice != -1) StopSoundEffectVoice(gOvl30SoundTestVoice);
                gOvl30SoundTestVoice = PlaySoundEffect(gOvl30SoundEffectIndex);
            } else {
                if (gPad0Input->repeatButtons & 0x2000) {
                    if (++gOvl30SoundEffectIndex >= 77) gOvl30SoundEffectIndex = 0;
                }
                if (gPad0Input->repeatButtons & 0x8000) {
                    if (--gOvl30SoundEffectIndex < 0) gOvl30SoundEffectIndex = 76;
                }
            }
            break;
        case 7:
            if (gPad0Input->pressedButtons & 0x20) {
                state = 999;
                StopSoundSequence(0);
                OpenSoundSequencesAndStart();
                RewindSoundLoadSlots(2, 0);
                QueueSoundLoadCommand(gOvl30SoundSequenceIndex + 1, 0);
            } else {
                if (gPad0Input->repeatButtons & 0x2000) {
                    if (++gOvl30SoundSequenceIndex >= 37) gOvl30SoundSequenceIndex = 0;
                }
                if (gPad0Input->repeatButtons & 0x8000) {
                    if (--gOvl30SoundSequenceIndex < 0) gOvl30SoundSequenceIndex = 35;
                }
            }
            break;
        case 8:
            if (gPad0Input->pressedButtons & 0x20) {
                if (gOvl30SoundTestVoice != -1) StopSoundEffectVoice(gOvl30SoundTestVoice);
                gOvl30SoundTestVoice = PlaySoundEffect(gOvl30AlternateSoundEffectIndex + 54);
            } else {
                if (gPad0Input->repeatButtons & 0x2000) {
                    if (++gOvl30AlternateSoundEffectIndex >= 16) gOvl30AlternateSoundEffectIndex = 0;
                }
                if (gPad0Input->repeatButtons & 0x8000) {
                    if (--gOvl30AlternateSoundEffectIndex < 0) gOvl30AlternateSoundEffectIndex = 15;
                }
            }
            break;
        case 9:
            if (gPad0Input->pressedButtons & 0x20) {
                StartCdAudioTrack(gOvl30CdAudioTrackIndex);
            } else {
                if (gPad0Input->repeatButtons & 0x2000) {
                    if (++gOvl30CdAudioTrackIndex >= 26) gOvl30CdAudioTrackIndex = 0;
                }
                if (gPad0Input->repeatButtons & 0x8000) {
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
    if (gPad0Input->repeatButtons & 0x2000) {
        gOvl30DebugSettingValue += 1;
        if (gOvl30DebugSettingValue == 0x100) {
            gOvl30DebugSettingValue = 0;
        }
    }

    if (gPad0Input->repeatButtons & 0x8000) {
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

/* Entry-time SDK console setup, before dispatching any page. */

/* Shared text-origin setter; also clears the two secondary pen offsets. */
void func_800183B8(s32 x, s32 y);

void Ovl30InitDebugFont(void) {
    s32 consoleId;

    FntLoad(0x2C0, 0x100);
    consoleId = FntOpen(0x18, 0, 0x100, 0xC8, 0, 0x200);
    SetDumpFnt(consoleId);
    func_800183B8(0x18, 0x18);
}
