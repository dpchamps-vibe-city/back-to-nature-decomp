#ifndef OVL15_CARD_READ_SPRITE_STATES_H
#define OVL15_CARD_READ_SPRITE_STATES_H

#include "common.h"

/* One update of the card-read / sprite-assisted slot-transfer scene.
 * Call once per menu frame while ovl_15 is resident, after the lifecycle has
 * initialized shared UI/card services. This is not a blocking whole-operation
 * call. gOvl15MenuState, selected card/slot, timers and menu choices carry state.
 *
 * Stages 0..33 poll cards, choose/read a save, check its checksums and header,
 * validate/copy selected slot data, and optionally write the result. I/O stages
 * retain control until their service completes. Other stages also monitor card
 * removal/change; selected-card == 2 alternates gOvl15MonitoredCard between the
 * two channels. Prompts use the text bank and active render context's packets.
 * The other read/write scenes share these globals: do not step them together.
 * No arguments or return value. Internal stage entries retain their linkage
 * but must not be called externally to bypass this protocol.
 * Original dispatcher: ovl_15:0x80131094. */
void Ovl15StepCardTransferMenu(void);

#endif
