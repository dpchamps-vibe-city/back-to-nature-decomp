/* Five-tier item-upgrade rules (ovl_11:8011DD90–8011DF4C).
 * The interaction service supplies two selected IDs. This unit classifies
 * their tier relationship and checks the per-category threshold; it neither
 * charges money nor changes an item. See ovl11_tier_upgrade_rules.h.
 * Membership and evidence-limited boundaries: notes/file-groupings.md. */
#include "common.h"
#include "ovl11_tier_upgrade_rules.h"

s32 Ovl11CheckTierUpgrade(void) {
    Ovl11UpgradeSelectionView *selection;
    s16 requestedTier;
    s16 currentTier;
    s32 category;
    s32 lowerTier;
    u16 progress;

    selection = (Ovl11UpgradeSelectionView *)&D_8006C838;
    requestedTier = selection->materialItemId;
    if ((u32)((selection->selectedItemId - 1) & 0xFFFF) >= 25U) {
        return 1;
    }
    category = (selection->selectedItemId - 1) / 5;
    currentTier = (selection->selectedItemId - 1) % 5;
    requestedTier = Ovl11GetUpgradeTierForItem(requestedTier);
    if (currentTier == 4) {
        return 2;
    }
    lowerTier = requestedTier < currentTier;
    if (lowerTier != 0) {
        return 3;
    }
    if (currentTier == requestedTier) {
        return 4;
    }
    /* Keep the signed category narrowing before the byte-stride calculation. */
    progress = *(u16 *)((u8 *)&D_8006C838->data[(category << 16) >> 15] + 0x5208);
    if ((s32)progress < Ovl11GetUpgradeValue(requestedTier, 0)) {
        return 5;
    }
    return 0;
}

s32 Ovl11GetItemTier(s16 itemId) {
    if ((u32)((itemId - 1) & 0xFFFF) >= 25U) {
        return -1;
    }
    return (itemId - 1) % 5;
}

s32 Ovl11GetUpgradeValue(s16 tier, s32 priceTable) {
    /* Only the low halfword selects the table. Values are zero-extended to
     * a signed word; the eligibility comparison must remain signed. */
    if ((priceTable << 16) == 0) {
        return gOvl11UpgradeThresholds[tier];
    }
    return gOvl11UpgradePrices[tier];
}

s32 Ovl11GetUpgradeTierForItem(s16 materialItemId) {
    if (materialItemId == 0x7A)
        return 3;
    if (materialItemId == 0x7B)
        return 2;
    if (materialItemId != 0x7C)
        return (materialItemId == 0x60) * 4;
    return 1;
}
