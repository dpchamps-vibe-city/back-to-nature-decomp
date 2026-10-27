/* Confirmation/message flow for releasing the last flag-0x20000 record
 * in the ten-entry small-record pool. The two local query helpers have no
 * other clients; shared interaction steps and engine work stay externally
 * owned. Public effects/preconditions: ovl11_selected_record_interaction.h.
 * Original ovl_11:80115CAC–80115FC8. */

#include "common.h"
#include "message_window.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_selected_record_interaction.h"
#include "ovl11_item_action_gate.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_small_record_handlers.h"
#include "two_choice_prompt.h"

void SetVal8005E2BC(s32 arg0);
s16 Ovl11FindLastFlaggedSmallRecord(void);
s32 Ovl11SelectedSmallRecordHasFlag40(void);

s32 Ovl11UpdateSelectedRecordRelease(void) {
    s32 result;
    s16 interactionId;

    result = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    interactionId = ((struct Ovl11TaggedItemState *)D_8006C838)->secondTag;
    switch (gOvl11InteractionStep) {
    case 0:
        Ovl11SetInteractionSteps(1, 0);
        break;
    case 1:
        if (interactionId == 0x10A) {
            RequestMessageWindow(2, 0x5C);
            Ovl11SetInteractionSteps(5, 7);
        } else if (interactionId == 0x109) {
            SetMessagePortraitEnabled(0);
            if (Ovl11SelectedSmallRecordHasFlag40() == 1) {
                RequestMessageWindow(2, 0x61);
            } else {
                RequestMessageWindow(2, 0x60);
            }
            Ovl11SetInteractionSteps(6, 2);
        }
        break;
    case 2:
        switch (Ovl11UpdateCompactChoicePrompt(gActiveRenderContext->field_D8 + 0x2C, 200, 128)) {
        case 0:
            break;
        case 1:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x51);
            if (Ovl11SelectedSmallRecordHasFlag40() == 1) {
                Ovl11AdjustSpendableBalance(2000, 1);
            } else {
                Ovl11AdjustSpendableBalance(500, 1);
            }
            Ovl11ClearSmallRecord((s32)((u8 *)&gSmallTaggedRecordBase + Ovl11FindLastFlaggedSmallRecord() * 0xB4));
            Ovl11SetInteractionSteps(5, 3);
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x5B);
            Ovl11SetInteractionSteps(5, 7);
            break;
        }
        break;
    case 3:
        Ovl11TryBeginItemAction(0);
        Ovl11SetInteractionSteps(4, 0);
        break;
    case 4:
        if (!(((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 & 0x10000)) {
            Ovl11SetInteractionSteps(7, 0);
        }
        break;
    case 5:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 6:
        if (GetMessageWindowStatus() == 5) {
            SetVal8005E2BC(0);
            Ovl11ResumeInteractionStep();
        }
        break;
    case 7:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}

/* Flags are at record+0x34, every 0xB4 bytes. Last match wins, not first. */
s16 Ovl11FindLastFlaggedSmallRecord(void) {
    s16 selected;
    s32 index;
    u32 *flags;
    u32 mask;
    char *base;

    selected = -1;
    index = 0;
    mask = 0x20000;
    base = (char *)&D_8006C838;
    base += 0x7AE8;
    flags = (u32 *)base;
    do {
        if (*flags & mask) {
            selected = index;
        }
        index += 1;
        flags += 0x2D;
    } while (index < 0xA);
    return selected;
}

/* This intentionally has no no-selection check; the handler requires one. */
s32 Ovl11SelectedSmallRecordHasFlag40(void) {
    s32 selected;
    u32 *flags;
    char *base;
    u32 word;

    selected = Ovl11FindLastFlaggedSmallRecord();
    base = (char *)&D_8006C838;
    base += 0x7AE8;
    flags = (u32 *)base;
    word = flags[selected * 0x2D] & 0x40;
    return word != 0;
}
