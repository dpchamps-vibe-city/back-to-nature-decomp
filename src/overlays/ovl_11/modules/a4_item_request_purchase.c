/* A4 item-request purchase callback (ovl_11:80119008).
 *
 * Interaction-table s32(void) interface, one call per update: 0 pending,
 * 1 finished/rejected. Requires resident ovl_11, initialized message/render/
 * primary-object services and valid secondary-item state. Global interaction
 * steps, drawing and the saved item ID make it non-reentrant.
 *
 * State 0 snapshots the secondary tag. The gate requires work+5234 bit
 * 10000000, a tag other than A4, and held-item class 1. Acceptance debits
 * A4's CURRENT price and removes a secondary single-slot match for the saved
 * tag. It then waits for work bit 04000000 before submitting {A4,savedId,1}
 * and polling completion. Rejection/cancellation does not debit or remove.
 * Every call clears work+522C/+5230 and asserts +5234 bit 800000; exit clears
 * that bit but leaves the saved ID intact. Message/ID meanings remain numeric.
 * No direct C caller: the callback bank publishes this entry's ABI.
 * Membership/boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "message_window.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_primary_object_wait_steps.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_cell_grid_lookup.h"
#include "two_choice_prompt.h"
#include "ovl11_item_slot_commands.h"

void SetVal8005E2BC(s32 arg0);

s32 Ovl11UpdateA4ItemRequestPurchase(void) {
    s32 result;
    s16 price;

    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    price = Ovl11GetItemPrice(0xA4);
    result = 0;
    switch (gOvl11InteractionStep) {
    case 0:
        gOvl11A4RequestItemId = ((struct Ovl11TaggedItemState *)D_8006C838)->secondTag;
        Ovl11SetInteractionSteps(1, 0);
        /* Validate the snapshot in the same update as entry. */
    case 1:
        if (((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 & 0x10000000) {
            if (gOvl11A4RequestItemId == 0xA4) {
                RequestMessageWindow(2, 0xBC);
                Ovl11SetInteractionSteps(2, 4);
            } else if (Ovl11ClassifyHeldItemType((u16)gOvl11A4RequestItemId) == 1) {
                RequestMessageWindow(2, 0xB6);
                SetMessagePortraitEnabled(0);
                Ovl11SetInteractionSteps(3, 5);
            } else {
                RequestMessageWindow(2, 0xB9);
                Ovl11SetInteractionSteps(2, 4);
            }
        } else {
            RequestMessageWindow(2, 0xB8);
            Ovl11SetInteractionSteps(2, 4);
        }
        break;
    case 2:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 3:
        if (GetMessageWindowStatus() == 5) {
            SetVal8005E2BC(0);
            Ovl11ResumeInteractionStep();
        }
        break;
    case 5:
        gOvl11InteractionDrawMode = 2;
        Ovl11SetInteractionSteps(6, 0);
        Ovl11UpdateCompactChoicePrompt(0, -1, 0);
        /* Initialize and poll the confirmation without an extra update. */
    case 6:
        switch (Ovl11UpdateCompactChoicePrompt(gActiveRenderContext->field_D8 + 0x2C, 0xE0, 0x80)) {
        case 0:
            break;
        case 1:
            if (Ovl11HasSpendableBalance(price) == 1) {
                CloseMessageWindow();
                RequestMessageWindow(2, 0xBB);
                Ovl11SetInteractionSteps(2, 7);
            } else {
                CloseMessageWindow();
                RequestMessageWindow(2, 0x4F);
                Ovl11SetInteractionSteps(2, 4);
            }
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0xB7);
            Ovl11SetInteractionSteps(2, 4);
            break;
        }
        break;
    case 7:
        gOvl11InteractionDrawMode = 0;
        Ovl11AdjustSpendableBalance(-price, 1);
        Ovl11MatchItemSlotsCommand(gOvl11A4RequestItemId, 1, 0, 1);
        Ovl11SetInteractionSteps(8, 4);
        break;
    case 8:
        if (((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 & 0x4000000) {
            Ovl11SetInteractionSteps(10, 0);
        }
        break;
    case 10:
    case 11:
        if (Ovl11StepPrimaryObjectA4Request(&gOvl11InteractionStep, 10, gOvl11A4RequestItemId)) {
            Ovl11SetInteractionSteps(4, 0);
        }
        break;
    case 4:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
