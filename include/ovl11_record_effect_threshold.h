#ifndef OVL11_RECORD_EFFECT_THRESHOLD_H
#define OVL11_RECORD_EFFECT_THRESHOLD_H

#include "common.h"
#include "game_types.h"

/* Read the SIGNED quantity at record+0x16. At 10 or above, start timed
 * sprite effect 0x25 for 0x2D updates in the effect state at +0xA8.
 * Below 10 nothing changes. The partial view does not establish a stride. */
void Ovl11StartRecordEffectAboveThreshold(Ovl11DF72CResetView *record);

#endif
