#ifndef OVL11_TAGGED_RECORD_PRESENTATION_H
#define OVL11_TAGGED_RECORD_PRESENTATION_H

#include "common.h"

/*
 * Selects the embedded sprite's animation from the record's +0x24/+0x22
 * selectors, applies a nonzero frame override when it differs, then draws
 * the sprite/marker. Afterwards sets sprite flag 0x100 for selector 0 or 5.
 * Requires a valid initialized 0xB8 record and the engine's render context.
 * This neither advances the record position nor allocates a record.
 */
void Ovl11DrawTaggedRecordSprite(u8 *record);

/* Add the three signed words at +0x48 to +0x38/+0x3C/+0x40, then clear
 * those delta words. Address is a signed word, preserving the caller ABI.
 * No region, collision or liveness check is made here. */
void Ovl11AdvanceTaggedRecordPosition(s32 recordAddress);

#endif
