/* Two-stage record-offer interaction: confirmation/value debits, fade and
 * request waits, then a message selected by a private ratio/level table.
 * Original ovl_11:80115FE0–801165C8. The selector is used only by this handler;
 * it is not a general ratio classifier. Contract: ovl11_record_offer.h. */

#include "common.h"
#include "game_types.h"
#include "screen_fade.h"
#include "message_window.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_primary_object_wait_steps.h"
#include "ovl11_record_offer.h"
#include "ovl11_selection_record_access.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "ovl11_selection_prompt_steps.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_cell_grid_lookup.h"

void SetVal8005E2BC(s32 arg0);
u16 Ovl11SelectOfferConditionMessage(void);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_8011D06C(s32 arg0);

s32 Ovl11UpdateRecordOfferInteraction(void) {
    Ovl11D8012D7B8 first;
    Ovl11D8012D7B8 second;
    s32 result;
    s16 price;

    first.unk4 = 2;
    first.unk6 = 0xDD;
    first.unk8 = 2;
    first.unkA = 0xDE;
    first.unk10 = 4;
    first.unk0 = gOvl11PrimarySelectionRecord;
    second.unk4 = 2;
    second.unk6 = 0xE6;
    second.unk8 = 2;
    second.unkA = 0xFC;
    second.unk10 = 6;
    second.unk0 = gOvl11SecondarySelectionRecord;
    price = Ovl11GetItemPrice(Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11SecondarySelectionRecord));
    result = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    switch (gOvl11InteractionStep) {
    case 0:
        Ovl11SetInteractionSteps(2, 0);
        break;
    case 1:
        first.unk4 = -1;
        Ovl11SetInteractionSteps(2, 0);
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        switch (Ovl11StepSelectionPrompt(&gOvl11InteractionStep, 2, first)) {
        case 0:
            break;
        case 3:
            Ovl11SetInteractionSteps(0x1A, 0);
            break;
        case 1:
            switch (((StructD548 *)gOvl11PrimarySelectionRecord)->index) {
            case 0:
                SetMessagePortraitEnabled(0);
                RequestMessageWindow(2, 0xE5);
                Ovl11SetInteractionSteps(0x19, 9);
                break;
            case 1:
                Ovl11SetInteractionSteps(0x10, 0);
                break;
            }
            break;
        case 2:
            break;
        }
        break;
    case 9:
        switch (ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x2C, 0xC8, 0x80)) {
        case 0:
            break;
        case 1:
            CloseMessageWindow();
            if (ovl_11_func_8011D06C(10) == 1) {
                Ovl11AdjustSpendableBalance(-10, 1);
                RequestMessageWindow(2, 0xE7);
                Ovl11SetInteractionSteps(0x18, 10);
            } else {
                RequestMessageWindow(2, 0x4F);
                Ovl11SetInteractionSteps(0x18, 1);
            }
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0xE8);
            Ovl11SetInteractionSteps(0x18, 1);
            break;
        }
        break;
    case 10:
        BeginScreenFadeIncrease(20, 1, 2);
        Ovl11SetInteractionSteps(11, 0);
        break;
    case 11:
        if (IsScreenFadeComplete() == 1) {
            RequestMessageWindow(2, 0xE9);
            Ovl11SetInteractionSteps(0x18, 12);
        }
        break;
    case 12:
        BeginScreenFadeDecrease(20, 1, 2);
        Ovl11SetInteractionSteps(13, 0);
        break;
    case 13:
        if (IsScreenFadeComplete() == 1) {
            RequestMessageWindow(2, 0xEA);
            Ovl11SetInteractionSteps(0x18, 14);
        }
        break;
    case 14:
        RequestMessageWindow(2, Ovl11SelectOfferConditionMessage());
        Ovl11SetInteractionSteps(0x18, 0x1A);
        break;
    case 15:
        second.unk4 = -1;
        Ovl11SetInteractionSteps(0x10, 0);
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
        switch (Ovl11StepSelectionPrompt(&gOvl11InteractionStep, 0x10, second)) {
        case 0:
            break;
        case 3:
            second.unk8 = -1;
            Ovl11SetInteractionSteps(1, 0);
            break;
        case 1:
            if (ovl_11_func_8011D06C(price) == 1) {
                Ovl11AdjustSpendableBalance(-price, 1);
                RequestMessageWindow(2, 0xFB);
                Ovl11SetInteractionSteps(0x18, 0x1B);
            } else {
                RequestMessageWindow(2, 0x4F);
                Ovl11SetInteractionSteps(0x18, 0x17);
            }
            break;
        case 2:
            break;
        }
        break;
    case 23:
        RequestMessageWindow(2, 0xFC);
        Ovl11SetInteractionSteps(0x18, 1);
        break;
    case 25:
        if (GetMessageWindowStatus() == 5) {
            SetVal8005E2BC(0);
            Ovl11ResumeInteractionStep();
        }
        break;
    case 24:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 26:
        result = 1;
        break;
    case 27:
    case 28:
        if (Ovl11StepPrimaryObjectItemRequest(&gOvl11InteractionStep, 27, ((StructD548 *)gOvl11SecondarySelectionRecord)->data[((StructD548 *)gOvl11SecondarySelectionRecord)->index])) {
            Ovl11SetInteractionSteps(26, 0);
        }
        break;
    }
    return result;
}

u16 Ovl11SelectOfferConditionMessage(void) {
    Ovl11OfferMessageState *base = (Ovl11OfferMessageState *)&D_8006C838;
    s16 ratio;
    s16 level;
    s32 row;
    s32 col;

    /* The assignment truncates the computed percentage to signed 16 bits. */
    ratio = base->numerator;
    ratio = (ratio * 100) / base->denominator;
    level = base->level;

    row = 0;
    if (ratio < 0x46) {
        if (ratio >= 0x32) {
            row = 1;
        } else if (ratio >= 0x14) {
            row = 2;
        } else if (ratio >= 5) {
            row = 3;
        } else {
            row = 4;
        }
    }

    col = 0;
    if (level >= 0x32) {
        if (level >= 0x46) {
            col = 3;
            if (level < 0x5A) {
                col = 2;
            }
        } else {
            col = 1;
        }
    }

    return gOvl11OfferConditionMessages[row][col];
}
