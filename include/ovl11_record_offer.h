#ifndef OVL11_RECORD_OFFER_H
#define OVL11_RECORD_OFFER_H

#include "common.h"

/* Interaction 0x14 update callback. Drives two selection/confirmation stages,
 * a 10-unit value debit, paired 20-step fades, condition messages and an
 * optional selected-item purchase/request. The selected price is stored as
 * s16 before affordability/debit. Return 1 at terminal step 26, otherwise 0.
 * Requires initialized primary/secondary selection records, shared interaction
 * steps, message/render/fade state and nonzero condition denominator.
 * Each call clears work+522C/+5230 and sets flag 0x800000 at +5234; unlike the
 * selected-record release handler, terminal completion does NOT clear it.
 * Shared state is not reentrant. The condition-message helper uses a private
 * 5x4 u16 table: percentage bands >=70/50/20/5/else, level bands <50/70/90/else.
 * Its computed percentage truncates to s16; no range/denominator guard exists. */
s32 Ovl11UpdateRecordOfferInteraction(void);

#endif
