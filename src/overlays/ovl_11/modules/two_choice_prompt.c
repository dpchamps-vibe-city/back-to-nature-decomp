/* Compact/wide two-choice drawing and the shared input protocol.
 * Wide selection persists here; compact selection is serviced by 80118B84.
 * Neither is newly allocated by this unit. See two_choice_prompt.h for the
 * sentinel-coordinate interface and notes/file-groupings.md for boundaries. */
#include "two_choice_prompt.h"
#include "beveled_panel.h"
#include "sound_commands.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libapi.h"
#include "psyq/libgpu.h"
#include "text_engine.h"

void Ovl11DrawChoicePromptVariant(s32 wide, s32 ot, s16 x, s16 y, s16 selection);

void Ovl11DrawCompactChoicePrompt(s32 ot, s16 x, s16 y, s16 selection) {
    Ovl11DrawChoicePromptVariant(0, ot, x, y, selection);
}

void Ovl11DrawWideChoicePrompt(s32 ot, s16 x, s16 y, s16 selection) {
    Ovl11DrawChoicePromptVariant(1, ot, x, y, selection);
}

void Ovl11DrawChoicePromptVariant(s32 wide, s32 ot, s16 x, s16 y, s16 selection) {
    s32 textTiles;
    s32 text;
    s32 cursorYOffset;

    if (wide != 0) {
        textTiles = 9;
        text = *D_80054BC0 + (s32) gBuyDeclineChoiceTextAnchor;
    } else {
        textTiles = 4;
        text = *D_80054BC0 + (s32) gYesNoChoiceTextAnchor;
    }
    cursorYOffset = selection * 0xE + 4;
    DrawChoiceCursor(ot, (s16) (x + 4), (s16) (y + cursorYOffset));
    SetTextSpacing(0, 2);
    DrawTextLine(ot, (s32) text, (s32) (s16) (x + 0xC), (s32) (s16) (y + 4));
    SetTextSpacing(0, 0);
    DrawBeveledPanel((u32 *) ot, x, y, (textTiles * 8) + 8, 0x24);
}

s32 Ovl11ProcessTwoChoiceInput(u16 *selection) {
    s32 pressed;
    s32 complete;
    u16 wrapped;
    u16 adjusted;

    complete = 0;
    pressed = *(s32 *) ((u8 *) gPad0Input + 8);
    if (pressed & 0x1000) {
        --*selection;
        PlaySoundEffect(5);
    } else if (pressed & 0x4000) {
        ++*selection;
        PlaySoundEffect(5);
    } else if (pressed & 0x40) {
        complete = 1;
        PlaySoundEffect((s16) *selection == 1);
    } else if (pressed & 0x20) {
        PlaySoundEffect(1);
        *selection = 1;
        complete = 1;
    }
    adjusted = *selection;
    if ((s16) *selection < 0) {
        adjusted = 1;
    }
    *selection = adjusted;
    wrapped = 0;
    if ((s16) adjusted < 2) {
        wrapped = adjusted;
    }
    *selection = wrapped;
    return complete;
}

s32 Ovl11UpdateWideChoicePrompt(s32 ot, s16 x, s16 y) {
    if (x == -1) {
        gOvl11WideChoiceSelection = 0;
        return 0;
    }
    if (x == -2) {
        gOvl11WideChoiceSelection = 1;
        return 0;
    }
    Ovl11DrawWideChoicePrompt(ot, x, y, gOvl11WideChoiceSelection);
    /* Preserve the helper's unsigned halfword edit / signed test contract. */
    if (Ovl11ProcessTwoChoiceInput((u16 *)&gOvl11WideChoiceSelection) == 1) {
        if (gOvl11WideChoiceSelection == 0) {
            gOvl11WideChoiceSelection = 0;
            return 1;
        }
        gOvl11WideChoiceSelection = 0;
        return 2;
    }
    return 0;
}
