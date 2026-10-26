/*
 * Memory-card menu UI: cursor choices, three-slot summaries, date formatting
 * and button-driven transitions. No card I/O is performed here.
 * Membership and evidence-limited outer boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl15_save_menu_ui.h"
#include "sound_commands.h"
#include "psyq/memory.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "beveled_panel.h"

void func_80017B3C(s32, s32, s32, s32);
void func_800248B0(s32, s16, s16);
void func_80017A08(s32, s32);
u16 *func_8001AA7C(s32, u16 *);
s16 *func_8001A970(s32, s16 *, s32);
void func_8001ABF0(u16 *, u16 *);
void func_800249C0(s32, s16, s16);
void Ovl15FormatDateNumber(s16, s16 *, s32);

s8 Ovl15UpdateTwoChoiceMenu(void) {
    s32 navigation;
    s32 buttons;

    DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x18), 1, 0x4E, 0x48, 0xA3, 0x22);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x14, (s32)((u8 *)D_80051D18 + gTextBankOffsets[0]), 0x5F, 0x4C);
    navigation = *(s32 *)((u8 *)gPad0Input + 0);
    if (navigation & 0x4000) {
        PlaySoundEffect(5);
        gOvl15MenuCursor = (u8)gOvl15MenuCursor + 1;
    } else if (navigation & 0x1000) {
        PlaySoundEffect(5);
        gOvl15MenuCursor = (u8)gOvl15MenuCursor - 1;
    }
    if (gOvl15MenuCursor < 0) {
        gOvl15MenuCursor = 1;
    }
    if (gOvl15MenuCursor >= 2) {
        gOvl15MenuCursor = 0;
    }
    func_800248B0(gActiveRenderContext->field_D8 + 4, 0x5C, (gOvl15MenuCursor * 0xD) + 0x52);
    buttons = *(s32 *)((u8 *)gPad0Input + 8);
    if (buttons & 0x40) {
        PlaySoundEffect(0);
        return gOvl15MenuCursor;
    }
    if (buttons & 0x20) {
        PlaySoundEffect(1);
        return -1;
    }
    return 2;
}

s8 Ovl15UpdateSaveSlotMenu(s16 displayMode) {
    s32 navigation;
    s32 buttons;
    s32 slot;

    for (slot = 0; slot < 3; slot++) {
        Ovl15DrawSaveSlotSummary(slot, displayMode);
    }
    navigation = *(s32 *)((u8 *)gPad0Input + 0);
    if (navigation & 0x4000) {
        PlaySoundEffect(5);
        gOvl15MenuCursor++;
    } else if (navigation & 0x1000) {
        PlaySoundEffect(5);
        gOvl15MenuCursor--;
    }
    if (gOvl15MenuCursor < 0) {
        gOvl15MenuCursor = 2;
    }
    if (gOvl15MenuCursor >= 3) {
        gOvl15MenuCursor = 0;
    }
    func_800248B0(gActiveRenderContext->field_D8 + 4, 0x39, (gOvl15MenuCursor * 0x31) + 0x18);
    buttons = *(s32 *)((u8 *)gPad0Input + 8);
    if (buttons & 0x40) {
        PlaySoundEffect(0);
        return gOvl15MenuCursor;
    }
    if (buttons & 0x20) {
        PlaySoundEffect(1);
        return -1;
    }
    return 3;
}

void Ovl15DrawSaveSlotSummary(s16 slot, s16 displayMode) {
    s16 *dayText;
    s16 *seasonText;
    s16 y;
    s16 status;
    s32 offset;
    s32 i;
    u16 glyph;
    struct Ovl15SaveHeader *header;
    void *headers;

    y = slot * 0x31 + 0x14;
    DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x18), 1, 0x30, (s32)(s16)(slot * 0x31 + 0x10), 0xE4, 0x30);
    func_80017A08(1, (s32)((void *)((u8 *)&gOvl15SaveSlotLabels + (slot * 4))));
    func_80017B3C(gActiveRenderContext->field_D8 + 0x14, (s32)((void *)((u8 *)&D_80051D8E + *gTextBankOffsets)), 0x41, (s32)y);
    status = gOvl15SaveSlotStatus[slot];
    switch (status) {
    case 1:
        func_80017B3C(gActiveRenderContext->field_D8 + 0x14, (s32)((void *)((u8 *)&D_80051D8E - 0x13A)) + gTextBankOffsets[0], 0x41, (s16)(y + 0x15));
        return;
    case 2:
        if (displayMode == 0) {
            func_80017B3C(gActiveRenderContext->field_D8 + 0x14, (s32)((void *)((u8 *)&D_80051D8E - 0x13A)) + gTextBankOffsets[0], 0x41, (s16)(y + 0x15));
        } else if (displayMode == 1) {
            func_80017B3C(gActiveRenderContext->field_D8 + 0x14, (s32)((void *)((u8 *)&D_80051D8E - 0x128)) + gTextBankOffsets[0], 0x41, (s16)(y + 0x15));
        }
        break;
    case 3:
        if (displayMode == 0) {
            func_80017B3C(gActiveRenderContext->field_D8 + 0x14, (s32)((void *)((u8 *)&D_80051D8E - 0x13A)) + gTextBankOffsets[0], 0x41, (s16)(y + 0x15));
        } else if (displayMode == 1) {
            func_80017B3C(gActiveRenderContext->field_D8 + 0x14, (s32)((void *)((u8 *)&D_80051D8E - 0xFC)) + gTextBankOffsets[0], 0x41, (s16)(y + 0x15));
        }
        break;
    case 0:
        /* Sanitize the loaded summaries in place, preserving special 0xFFD.
         * The ninth glyph of each name is terminated below. */
        for (i = 0; i < 8; i++) {
            glyph = gOvl15SaveSlotHeaders[slot].name_8.units[i];
            if ((glyph >= 0xE01U) && (glyph != 0xFFD)) {
                gOvl15SaveSlotHeaders[slot].name_8.units[i] = 0xFFFF;
            }
        }
        for (i = 0; i < 8; i++) {
            glyph = gOvl15SaveSlotHeaders[slot].name_1A.units[i];
            if ((glyph >= 0xE01U) && (glyph != 0xFFD)) {
                gOvl15SaveSlotHeaders[slot].name_1A.units[i] = 0xFFFF;
            }
        }
        y += 0xE;
        memset(gOvl15MenuTextScratch, 0xFF, 0x28);
        headers = gOvl15SaveSlotHeaders;
        offset = slot * sizeof(struct Ovl15SaveHeader);
        header = (void *)((u8 *)headers + offset);
        Ovl15FormatDateNumber((s16)(((u16 *)&header->words)[0] + 1), gOvl15MenuTextScratch, 2);
        func_80017A08(2, (s32)gOvl15MenuTextScratch);
        seasonText = gOvl15MenuTextScratch + 5;
        *(s16 *)func_8001AA7C((s32)((s16 *)&header->words)[1], (u16 *)seasonText) = -1;
        func_80017A08(3, (s32)seasonText);
        dayText = gOvl15MenuTextScratch + 0xD;
        Ovl15FormatDateNumber((s16)(((u16 *)&header->words)[2] + 1), dayText, 2);
        func_80017A08(4, (s32)dayText);
        header->name_8.units[8] = 0xFFFF;
        func_80017A08(5, (s32)gOvl15SaveSlotHeaders[slot].name_8.units);
        func_80017B3C(gActiveRenderContext->field_D8 + 0x14, (s32)((void *)((u8 *)&D_80051D9E + *gTextBankOffsets)), 0x41, (s32)y);
        y += 0xE;
        func_80017A08(6, (s32)gOvl15SaveSlotHeaders[slot].name_1A.units);
        header->name_1A.units[8] = 0xFFFF;
        func_80017B3C(gActiveRenderContext->field_D8 + 0x14, (s32)((void *)((u8 *)((void *)((u8 *)&D_80051D9E + 0x1C)) + *gTextBankOffsets)), 0x41, (s32)y);
        return;
    }
}

/* Append a relocated suffix selected by 1/2/3/other after numeric formatting.
 * The caller supplies pre-terminated halfword text storage. */
void Ovl15FormatDateNumber(s16 value, s16 *text, s32 numberMode) {
    s32 offset;

    if (value == 1) {
        offset = 0x12;
    } else if (value == 2) {
        offset = 0x18;
    } else if (value == 3) {
        offset = 0x1E;
    } else {
        offset = 0x24;
    }
    func_8001ABF0((u16 *)func_8001A970(value, text, numberMode),
                  (u16 *)(gTextBankOffsets[0] + offset + (s32)D_8005175C));
}

s8 Ovl15UpdateConfirmationMenu(void) {
    s32 buttons;
    s32 navigation;

    func_80017B3C(gActiveRenderContext->field_D8 + 8, (s32)(D_80051D80 + gTextBankOffsets[0]), 0x2B, 0xC4);
    /* Unlike the preceding menus, confirmation precedes navigation. */
    buttons = *(s32 *)((u8 *)gPad0Input + 8);
    if (buttons & 0x40) {
        if (gOvl15ConfirmationChoice == 0) {
            PlaySoundEffect(0);
        } else {
            PlaySoundEffect(1);
        }
        return gOvl15ConfirmationChoice;
    }
    if (buttons & 0x20) {
        PlaySoundEffect(1);
        return -1;
    }
    navigation = *(s32 *)((u8 *)gPad0Input + 0);
    if (navigation & 0x4000) {
        PlaySoundEffect(5);
        gOvl15ConfirmationChoice = (u8)gOvl15ConfirmationChoice + 1;
    } else if (navigation & 0x1000) {
        PlaySoundEffect(5);
        gOvl15ConfirmationChoice = (u8)gOvl15ConfirmationChoice - 1;
    }
    if (gOvl15ConfirmationChoice < 0) {
        gOvl15ConfirmationChoice = 1;
    }
    if (gOvl15ConfirmationChoice >= 2) {
        gOvl15ConfirmationChoice = 0;
    }
    func_800248B0(gActiveRenderContext->field_D8 + 4, 0x23, (gOvl15ConfirmationChoice * 0xE) + 0xC8);
    return 2;
}

s32 Ovl15WaitForMenuButton(s16 waitingState, s16 pressedState) {
    func_800249C0(gActiveRenderContext->field_D8 + 4, 0x11D, 0xD9);
    if (((GfxObj *)gPad0Input)->field_8 == 0) {
        return waitingState;
    } else {
        PlaySoundEffect(0);
        return pressedState;
    }
}
