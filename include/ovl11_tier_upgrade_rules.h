#ifndef OVL11_TIER_UPGRADE_RULES_H
#define OVL11_TIER_UPGRADE_RULES_H

#include "common.h"

/* ovl_11:8011DD90. Read selected item IDs at work offsets 524C/5252 and
 * the category's u16 progress at 5208. Return 0 if eligible, 1 for an ID
 * outside 1..25, 2 at the final tier, 3 for a lower requested tier, 4 for
 * the same tier, or 5 for insufficient progress. No state is changed.
 * The 25 IDs encode five categories, each with five consecutive tiers. */
s32 Ovl11CheckTierUpgrade(void);

/* ovl_11:8011DE70. Return tier 0..4 for IDs 1..25, otherwise -1. */
s32 Ovl11GetItemTier(s16 itemId);

/* ovl_11:8011DEBC. Unchecked tier index: callers must supply 0..4.
 * Low 16 bits of priceTable == 0 selects progress thresholds, otherwise
 * prices in the interaction service's money units. Return a zero-extended
 * u16 as s32, including threshold 65535 at tier 4. No state is changed. */
s32 Ovl11GetUpgradeValue(s16 tier, s32 priceTable);

/* ovl_11:8011DF04. Material IDs 7C/7B/7A/60 request tiers 1/2/3/4;
 * every other ID maps to 0. These are numeric IDs, not named commodities. */
s32 Ovl11GetUpgradeTierForItem(s16 materialItemId);

#endif
