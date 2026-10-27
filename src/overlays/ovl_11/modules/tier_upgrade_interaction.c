/* Tier-upgrade purchase callback (ovl_11:8011A25C).
 *
 * Table-driven s32(void) interface: call once per update with resident ovl_11,
 * initialized messages/render context and valid selected category/item IDs.
 * Returns 0 while waiting, 1 on completion or rejection. Uses the GLOBAL
 * interaction step/resume/draw state; cannot run alongside another interaction.
 * Prices and validation are re-read on every call: selections must stay stable.
 *
 * Two confirmations precede debit, progress action and slot replacement.
 * Success fades for 20 updates each way, requests area 32, and installs raw
 * position (1000,0,-2800). Rejection still runs the message/flag cleanup path.
 * Entry clears work+522C/+5230 and asserts +5234 bit 800000; completion clears
 * that bit. State 12 is a debit phase, not a repeat-safe public entry point.
 * Membership/boundaries: notes/file-groupings.md. No private linkage inferred.
 */

#include "common.h"
#include "screen_fade.h"
#include "message_window.h"
#include "ovl11_tier_upgrade_rules.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_progress_status.h"
#include "ovl11_balance_accounting.h"
#include "two_choice_prompt.h"
#include "ovl11_item_slot_commands.h"
#include "ovl11_object_event_slot_reset_command.h"
#include "ovl11_object_state_selection_command.h"
#include "ovl11_area_selection_command.h"

void SetVal8005E2BC(s32 arg0);

s32 Ovl11UpdateTierUpgradePurchase(void) {
    s32 result;
    s16 item;
    s16 group;
    s32 price;
    s32 index;
    s32 row;
    s16 message;

    result = 0;
    item = ((Ovl11TierUpgradeWork *)D_8006C838)->selectedItemId;
    group = ((Ovl11TierUpgradeWork *)D_8006C838)->categoryItemId;
    ((Ovl11TierUpgradeWork *)D_8006C838)->field_522C = 0;
    ((Ovl11TierUpgradeWork *)D_8006C838)->field_5230 = 0;
    ((Ovl11TierUpgradeWork *)D_8006C838)->flags |= 0x800000;
    row = (s16)Ovl11GetUpgradeTierForItem(item);
    price = Ovl11GetUpgradeValue(row, 1);
    switch (gOvl11InteractionStep) {
    case 0:
        Ovl11SetInteractionSteps(3, 0);
        break;
    case 1:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 2:
        if (GetMessageWindowStatus() == 5) {
            SetVal8005E2BC(0);
            Ovl11ResumeInteractionStep();
        }
        break;
    case 15:
        if (((Ovl11TierUpgradeWork *)D_8006C838)->flags & 0x10000000) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 3:
        message = Ovl11CheckTierUpgrade();
        if (message != 0) {
            RequestMessageWindow(2, gOvl11UpgradeFailureMessages[message - 1]);
            Ovl11SetInteractionSteps(1, 4);
        } else {
            gOvl11InteractionDrawMode = 2;
            SetMessagePortraitEnabled(0);
            RequestMessageWindow(2, Ovl11GetItemTier(Ovl11GetUpgradeTierForItem(item)) + 0x9A);
            Ovl11SetInteractionSteps(2, 5);
        }
        break;
    case 5:
        Ovl11SetInteractionSteps(6, 0);
        Ovl11UpdateCompactChoicePrompt(0, -1, 0);
        /* Start and poll the first confirmation in the same update. */
    case 6:
        switch (Ovl11UpdateCompactChoicePrompt(gActiveRenderContext->field_D8 + 0x2C, 200, 0x80)) {
        case 0:
            break;
        case 1:
            CloseMessageWindow();
            if (Ovl11HasSpendableBalance(price) == 1) {
                SetMessagePortraitEnabled(0);
                RequestMessageWindow(2, 0x9E);
                Ovl11SetInteractionSteps(2, 13);
            } else {
                RequestMessageWindow(2, 0x4F);
                Ovl11SetInteractionSteps(1, 4);
            }
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x93);
            Ovl11SetInteractionSteps(1, 4);
            break;
        }
        break;
    case 13:
        Ovl11SetInteractionSteps(14, 0);
        Ovl11UpdateCompactChoicePrompt(0, -1, 0);
        /* The second confirmation uses a separate prompt position. */
    case 14:
        switch (Ovl11UpdateCompactChoicePrompt(gActiveRenderContext->field_D8 + 0x2C, 0xE0, 0x80)) {
        case 0:
            break;
        case 1:
            CloseMessageWindow();
            Ovl11SetInteractionSteps(11, 0);
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x93);
            Ovl11SetInteractionSteps(1, 4);
            break;
        }
        break;
    case 11:
        gOvl11InteractionDrawMode = 0;
    case 12:
        Ovl11AdjustSpendableBalance(-price, 1);
        index = Ovl11GetUpgradeTierForItem(item) + 1;
        row = (group - 1) / 5 * 4;
        row += (group - 1) / 5;
        row += index;
        Ovl11RequestProgressAction(row);
        CloseMessageWindow();
        RequestMessageWindow(2, 0x9F);
        Ovl11SetInteractionSteps(1, 10);
        break;
    case 10:
        Ovl11MatchItemSlotsCommand(item, 1, 0, 1);
        Ovl11MatchItemSlotsCommand(group, 0, 0, 1);
        BeginScreenFadeIncrease(20, 0, 2);
        Ovl11SetInteractionSteps(9, 0);
    case 9:
        if (IsScreenFadeComplete() == 1) {
            Ovl11RequestAreaSelectionCommand(32, 0, 0, 0);
            ((Ovl11TierUpgradeWork *)D_8006C838)->x = 1000;
            ((Ovl11TierUpgradeWork *)D_8006C838)->z = -2800;
            ((Ovl11TierUpgradeWork *)D_8006C838)->y = 0;
            Ovl11SelectObjectStateOrDirectionCommand(3, 0, 0, 0);
            BeginScreenFadeDecrease(20, 0, 2);
            Ovl11SetInteractionSteps(8, 0);
        }
        break;
    case 4:
        RequestMessageWindow(2, 0x87);
        Ovl11SetInteractionSteps(1, 7);
        break;
    case 8:
        if (IsScreenFadeComplete() != 1) {
            break;
        }
        Ovl11ResetObjectEventSlotCommand(4, 0, 0, 0);
        Ovl11SetInteractionSteps(7, 0);
    case 7:
        ((Ovl11TierUpgradeWork *)D_8006C838)->flags &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
