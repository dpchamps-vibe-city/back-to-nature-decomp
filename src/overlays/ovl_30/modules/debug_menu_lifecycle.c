/* Debug-menu lifecycle and private display preservation. The display helper
 * is a real nested definition: that scope emits its original static-chain
 * spill and positions it before the parent without a register-pin workaround.
 * Membership and evidence-limited boundaries: notes/file-groupings.md. */
#include "common.h"
#include "game_types.h"
#include "ovl30_debug_menu_lifecycle.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"
#include "scene_selection.h"
#include "cd_audio.h"
#include "master_volume.h"
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
        buttons = *(s32 *)((u8 *)gPad0Input + 8);
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
                if ((*(s32 *)((u8 *)gPad0Input + 8)) & 0x800) {
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
                StoreImage((RECT *)Ovl30_DisplayRect, pixels);
            } else {
                LoadImage((RECT *)Ovl30_DisplayRect, pixels);
            }
            DrawSync(0);
        }
        copy_display(1);
        SetGameFlag(0x9CU, 0U);
        index = &gOvl30DebugSettingValue;
        if (*index != ((struct Ovl30SettingsView *)&D_8007AFF0)->selected) {
            SetGameFlag(0x9CU, 1U);
            D_8005E32C = *index;
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
