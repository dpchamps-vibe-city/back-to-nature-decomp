/* Four-slot contextual messages and their three-image presentation cycle.
 * Slot 2 also drives the follow-up message and the nine-cell confirmation
 * protocol. No gameplay identity is inferred from the work-area indices.
 * See ovl11_choice_messages.h and notes/file-groupings.md. */
#include "ovl11_choice_messages.h"
#include "message_window.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_upload.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/libgs.h"
#include "ovl11_area_resource_load.h"

void ovl_11_func_800BCF28(void);
void Ovl11BuildChoiceMessageSlots(void);
s32 Ovl11HasUniformChoiceMessages(void);
s32 Ovl11TryFillUniformChoiceMessages(s16 selectedKind, s16 value);
void Ovl11SetChoiceMessageCategory(s16 category, s16 group);
void Ovl11SetChoiceMessageGridEntries(s16 row, s16 column);
void Ovl11SetChoiceMessageVariant(s16 section, s16 row, s16 column, s16 ruleIndex);
s32 Ovl11TrySetSpecialChoiceMessage(s16 row, s16 column);
s32 Ovl11TrySetCellChoiceMessage(void);
s32 Ovl11GetChoiceMessageBand(s16 value, s32 bandCount);
void Ovl11ProcessChoiceMessageInput(void);
s32 Ovl11SelectChoiceMessageFromPad(void);
s32 Ovl11RequestChoiceVariantMessage(void);
s32 Ovl11GetChoiceVariantMessage(void);
s32 Ovl11RequestSelectedChoiceMessage(void);
void Ovl11SelectChoiceImageOffsets(void);
void Ovl11LoadChoiceImages(void);
void Ovl11CycleChoiceImages(void);
void Ovl11RestoreChoiceImages(void);
void Ovl11AdvanceChoiceCellStates(void);
void Ovl11ConfirmChoiceCellState(void);

void Ovl11ResetChoiceMessages(void) {
    Ovl11ClearChoiceMessages();
}

void Ovl11RequestChoiceMessages(void) {
    if (gOvl11ChoiceMessagePhase == 0) {
        gOvl11ChoiceMessagePhase = 1;
    }
}

void Ovl11UpdateChoiceMessages(void) {
    char *farBase;
    s16 *context;
    s32 status;
    s32 nextPhase;

    farBase = (char *)&D_8007AFF0;
    context = *(s16 **)(farBase + 0x25388);
    if (context[1] != 2) {
        return;
    }
    switch (gOvl11ChoiceMessagePhase) {
    case 0:
        break;
    case 1:
        Ovl11BuildChoiceMessageSlots();
        Ovl11SelectChoiceImageOffsets();
        if (Ovl11HasUniformChoiceMessages() == 1) {
            nextPhase = 3;
        } else {
            Ovl11RequestChoiceVariantMessage();
            nextPhase = 2;
        }
        gOvl11ChoiceMessagePhase = nextPhase;
        break;
    case 2:
        status = GetMessageWindowStatus();
        if (status == 5) {
            if (Ovl11SelectChoiceMessageFromPad() == 1) {
                CloseMessageWindow();
            }
        } else if (status == 2) {
            gOvl11ChoiceMessagePhase = 3;
        }
        break;
    case 3:
        CloseMessageWindow();
        Ovl11LoadChoiceImages();
        Ovl11RequestSelectedChoiceMessage();
        nextPhase = 4;
        gOvl11ChoiceMessagePhase = nextPhase;
        break;
    case 4:
        status = GetMessageWindowStatus();
        if (status == 5) {
            Ovl11ConfirmChoiceCellState();
        }
        if (status == 2) {
            gOvl11ChoiceMessagePhase = 5;
        }
        if (status != 14) {
            Ovl11ProcessChoiceMessageInput();
        }
        Ovl11CycleChoiceImages();
        break;
    case 5:
        Ovl11RestoreChoiceImages();
        Ovl11ClearChoiceMessages();
        ovl_11_func_800BCF28();
        break;
    case 6:
        if (GetMessageWindowStatus() == 2) {
            nextPhase = 5;
            gOvl11ChoiceMessagePhase = nextPhase;
        }
        break;
    }
}

void Ovl11ClearChoiceMessages(void) {
    gOvl11ChoiceMessageIndex = 0;
    gOvl11ChoiceMessagePhase = 0;
}

void Ovl11BuildChoiceMessageSlots(void) {
    char *work;

    work = (char *)D_8006C838;
    if (Ovl11TryFillUniformChoiceMessages(*(s16 *)(work + 0xE4C8), *(s16 *)(work + 0x44C0)) != 1) {
        Ovl11SetChoiceMessageCategory(*(s16 *)(work + 0xE4CA), *(s16 *)(work + 0x44BA));
        Ovl11SetChoiceMessageGridEntries(*(s16 *)(work + 0x44BA), *(s16 *)(work + 0x44BC));
        Ovl11SetChoiceMessageVariant(*(s16 *)(work + 0x44B8), *(s16 *)(work + 0x44BA), *(s16 *)(work + 0x44BC), *(s16 *)(work + 0x44BE));
    }
}

s32 Ovl11TryFillUniformChoiceMessages(s16 selectedKind, s16 value) {
    s32 messageEntry;
    s32 variant;
    Ovl11ChoiceMessageSlot *slot;
    s32 i;

    messageEntry = 0;
    if ((u32)value < 6U) {
        messageEntry = 0x26B;
    }
    if ((u32)(selectedKind - 3) < 2U) {
        messageEntry = 0x26A;
    }
    if (messageEntry) {
        if (messageEntry == 0x26A) {
            variant = 0x11;
        } else {
            variant = 0x10;
        }
        slot = &gOvl11ChoiceMessageSlots[0];
        for (i = 3; i >= 0; i--) {
            slot->messageEntry = messageEntry;
            slot->variant = variant;
            slot++;
        }
        return 1;
    }
    return 0;
}

s32 Ovl11HasUniformChoiceMessages(void) {
    return (u32)((u16)gOvl11ChoiceMessageSlots[0].variant - 0x10) < 2U;
}

void Ovl11SetChoiceMessageCategory(s16 category, s16 group) {
    s16 entry;
    s16 variant;

    if (category >= 3) {
        if (category == 3) {
            entry = 0x270;
            variant = 7;
        } else {
            entry = 0x275;
            variant = 8;
        }
    } else {
        entry = gOvl11ChoiceCategoryMessages[group];
        variant = 4;
        if (category > 0) {
            entry = entry + 1;
            if (category == 1) {
                variant = 5;
            } else {
                variant = 6;
            }
        }
    }
    gOvl11ChoiceMessageSlots[0].messageEntry = entry;
    gOvl11ChoiceMessageSlots[0].variant = variant;
}

void Ovl11SetChoiceMessageGridEntries(s16 row, s16 column) {
    s32 index;

    index = row * 0x1E + column;
    gOvl11ChoiceMessageSlots[1].messageEntry = (s16)(index + 0x277);
    gOvl11ChoiceMessageSlots[1].variant = 0;
    gOvl11ChoiceMessageSlots[3].messageEntry = (s16)(index + 0x2EF);
    gOvl11ChoiceMessageSlots[3].variant = 1;
}

void Ovl11SetChoiceMessageVariant(s16 section, s16 row, s16 column, s16 ruleIndex) {
    s16 variant;
    s16 value;
    s16 entry;
    Ovl11ChoiceVariantRule *rule;

    if (Ovl11TrySetSpecialChoiceMessage(row, column) != 1) {
        rule = &gOvl11ChoiceVariantRules[ruleIndex];
        value = (section * 0x71) - (5 - (column + (row * 0x1E)));
        entry = rule->baseEntry + Ovl11GetChoiceMessageBand(value, (s32)rule->bandCount);
        variant = rule->variant;
        switch (ruleIndex) {
        case 5:
            if (value >= 0x224) {
                entry += 1;
            }
            break;
        case 6:
            if (Ovl11TrySetCellChoiceMessage() == 1) {
                return;
            }
            if ((section == ruleIndex) && (row == 3)) {
                if (column == 0xF) {
                    entry = 0x182;
                }
                if (column == 0x16) {
                    entry = 0x178;
                }
            }
            break;
        }
        gOvl11ChoiceMessageSlots[2].messageEntry = entry;
        gOvl11ChoiceMessageSlots[2].variant = variant;
    }
}

s32 Ovl11TrySetSpecialChoiceMessage(s16 row, s16 column) {
    s16 entry;

    entry = 0;
    if (row == 3) {
        if (column >= 0x1C) {
            entry = (s16)(column + 0x24B);
        }
    } else if ((row == 0) && (column < 5)) {
        entry = (s16)(column + 0x262);
    }
    if (entry != 0) {
        gOvl11ChoiceMessageSlots[2].messageEntry = entry;
        gOvl11ChoiceMessageSlots[2].variant = 0xA;
        return 1;
    }
    return 0;
}

s32 Ovl11TrySetCellChoiceMessage(void) {
    char *farBase;
    u8 *work;
    u8 *states;
    s32 i;

    farBase = (char *)&D_8007AFF0;
    if ((u32)(*(s32 *)(*(s32 *)(farBase + 0x25388) + 4) - 0x50) < 4U) {
        return 0;
    }
    Ovl11AdvanceChoiceCellStates();
    i = 0;
    work = D_80074838;
    /* This is the same nine-byte region addressed as D_8007AFDA below. */
    states = work + 0x67A2;
    for (; i < 9; i++) {
        if ((u32)(states[i] - 1) < 3U) {
            gOvl11ChoiceMessageSlots[2].messageEntry = (s16)(i + 0x258);
            gOvl11ChoiceMessageSlots[2].variant = 2;
            return 1;
        }
    }
    return 0;
}

s32 Ovl11GetChoiceMessageBand(s16 value, s32 bandCount) {
    s16 band;
    s16 count;

    count = (s16)bandCount;
    band = (s16)(value % (s32)(count * 7));
    band = band / 7;
    if (band >= count) {
        band = count - 1;
    }
    return band;
}

void Ovl11ProcessChoiceMessageInput(void) {
    s32 buttons;
    s32 selected;

    buttons = ((GfxObj *)gPad0Input)->field_8 & 0xF020;
    if (buttons == 0) {
        return;
    }
    if (buttons == 0x20) {
        gOvl11ChoiceMessagePhase = 6;
        CloseMessageWindow();
        return;
    }
    if (buttons == 0x1000) {
        selected = 0;
    } else if (buttons == 0x8000) {
        selected = 2;
    } else if (buttons == 0x2000) {
        selected = 1;
    } else if (buttons == 0x4000) {
        selected = 3;
    } else {
        selected = gOvl11ChoiceMessageIndex;
    }
    if (gOvl11ChoiceMessageIndex != selected) {
        gOvl11ChoiceMessageIndex = selected;
        CloseMessageWindow();
        gOvl11ChoiceMessagePhase = 2;
    }
}

s32 Ovl11SelectChoiceMessageFromPad(void) {
    s32 buttons = ((GfxObj *)gPad0Input)->field_8 & 0xF020;

    if (buttons == 0x1000) {
        gOvl11ChoiceMessageIndex = 0;
    } else if (buttons == 0x8000) {
        gOvl11ChoiceMessageIndex = 2;
    } else if (buttons == 0x2000) {
        gOvl11ChoiceMessageIndex = 1;
    } else if (buttons == 0x4000) {
        gOvl11ChoiceMessageIndex = 3;
    } else {
        return 0;
    }
    return 1;
}

s32 Ovl11RequestChoiceVariantMessage(void) {
    s32 entry;
    s32 result;

    entry = Ovl11GetChoiceVariantMessage();
    CloseMessageWindow();
    result = RequestMessageWindow(3, entry);
    return result;
}

s32 Ovl11GetChoiceVariantMessage(void) {
    switch ((s16)(gOvl11ChoiceMessageSlots[2].variant - 2)) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    default:
        return 0x39D;
    case 1:
        return 0x399;
    case 7:
        return 0x39E;
    case 8:
        if (gOvl11ChoiceMessageSlots[2].messageEntry >= 0x267) {
            return 0x3A0;
        }
        return 0x3A1;
    case 9:
        return 0x39C;
    case 10:
        return 0x39A;
    case 11:
        return 0x39B;
    case 12:
        return 0x39F;
    case 13:
        return 0x398;
    case 14:
        return 0x26B;
    case 15:
        return 0x26A;
    }
}

s32 Ovl11RequestSelectedChoiceMessage(void) {
    return RequestMessageWindow(3, gOvl11ChoiceMessageSlots[gOvl11ChoiceMessageIndex].messageEntry);
}

void Ovl11SelectChoiceImageOffsets(void) {
    char *farBase = (char *)&D_8007AFF0;
    u32 *context;
    s32 mode;

    context = *(u32 **)(farBase + 0x25388);
    mode = *(s32 *)((u8 *)context + 4);
    if ((u32)(mode - 0x58) < 4) {
        gOvl11ChoiceImageOffsets.x = 0x15A;
        gOvl11ChoiceImageOffsets.y = 0x60;
        gOvl11ChoiceImageOffsets.type2X = 0x140;
        gOvl11ChoiceImageOffsets.type2Y = 0x8F;
    } else {
        gOvl11ChoiceImageOffsets.x = 0x140;
        gOvl11ChoiceImageOffsets.y = 1;
        gOvl11ChoiceImageOffsets.type2X = 0x140;
        gOvl11ChoiceImageOffsets.type2Y = -0x30;
    }
}

void Ovl11LoadChoiceImages(void) {
    gOvl11ChoiceImageTick = 0;
    gOvl11ChoiceImageBlock = -1;
    Ovl11LoadSmallIndexedSpriteBank(gOvl11ChoiceMessageSlots[gOvl11ChoiceMessageIndex].variant);
}

/* Three 0x800-byte image blocks, each held for 16 updater calls. Upload only
 * when the selected block changes; signed division behavior is retained. */
void Ovl11CycleChoiceImages(void) {
    s32 block;
    s32 adjustedTick;

    adjustedTick = gOvl11ChoiceImageTick;
    if (gOvl11ChoiceImageTick < 0) {
        adjustedTick = gOvl11ChoiceImageTick + 0xF;
    }
    block = (adjustedTick >> 4) - ((gOvl11ChoiceImageTick / 48) * 3);
    if (gOvl11ChoiceImageBlock != block) {
        gOvl11ChoiceImageBlock = block;
        UploadSpriteImagesAtOffsets((D_8007BFF8 + (block << 0xB)), gOvl11ChoiceImageOffsets.x, gOvl11ChoiceImageOffsets.y, gOvl11ChoiceImageOffsets.type2X, (s16)(s32)gOvl11ChoiceImageOffsets.type2Y);
    }
    gOvl11ChoiceImageTick += 1;
}

void Ovl11RestoreChoiceImages(void) {
    Ovl11LoadSmallIndexedSpriteBank(0x12);
    UploadSpriteImagesAtOffsets(((u8 *)(((s32)(&D_8007BFF8)))), gOvl11ChoiceImageOffsets.x, gOvl11ChoiceImageOffsets.y, gOvl11ChoiceImageOffsets.type2X, gOvl11ChoiceImageOffsets.type2Y);
}

void Ovl11AdvanceChoiceCellStates(void) {
    char *workBase;
    char *upperWork;
    u8 *states;
    u16 key;
    s32 i;
    s32 previous;

    states = &gChoiceCellStates[0];
    workBase = (char *)&D_8006C838;
    key = (*(u16 *)(workBase + 0x44BC) + (*(s16 *)(workBase + 0x44B8) * 0x78 + *(s16 *)(workBase + 0x44BA) * 0x1E)) & 0xFFFF;
    upperWork = workBase + 0x8000;
    if (key != (*(u16 *)(upperWork + 0x67A0))) {
        (*(u16 *)(upperWork + 0x67A0)) = key;
        previous = -1;
        for (i = 0; i < 9; i++) {
            switch (states[i]) {
            case 1:
            case 2:
                previous = i;
                states[i] = 0;
                break;
            case 3:
                previous = i;
                states[i] = 0xFF;
                break;
            }
            if (previous != -1) {
                break;
            }
        }
        if (previous == -1) {
            previous = 0;
        } else {
            previous = (previous + 1) % 9;
        }
        for (i = previous; i < 9; i++) {
            if (states[i] == 0) {
                states[i] = 1;
                return;
            }
        }
        for (i = 0; i <= previous; i++) {
            if (states[i] == 0) {
                states[i] = 1;
                return;
            }
        }
    }
}

void Ovl11ConfirmChoiceCellState(void) {
    s16 variant;
    s32 cell;

    variant = gOvl11ChoiceMessageSlots[2].variant;
    if (variant == 2) {
        cell = gOvl11ChoiceMessageSlots[2].messageEntry - 600;
        if ((u32)cell < 9U) {
            if (((Ovl11StatesE7A2View *)D_8006C838)->states[cell] == 1) {
                ((Ovl11StatesE7A2View *)D_8006C838)->states[cell] = variant;
            }
        }
    }
}
