/* Set up the text/quantity/value descriptor shared by quantity prompts.
 * This does not reset the whole record: zero quantity and a nonzero text
 * preservation flag intentionally keep previous values. No drawing occurs.
 * Membership and evidence-limited boundaries: notes/file-groupings.md. */
#include "common.h"
#include "game_types.h"
#include "ovl11_quantity_prompt_setup.h"

void Ovl11SetupQuantityPrompt(Ovl11QuantityPromptView *prompt, s32 primaryTextAddress,
                       s16 preserveSecondaryText, s16 unitValue, s16 quantity) {
    prompt->primaryTextAddress = primaryTextAddress;
    prompt->unitValue = unitValue;
    if (quantity) {
        prompt->quantity = quantity;
    }
    if (preserveSecondaryText == 0) {
        prompt->secondaryTextAddress = D_80054BC0[0] + (s32)&gQuantityPromptDefaultTextAnchor;
    }
}
