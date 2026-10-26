#ifndef OVL15_CARD_READ_STATES_H
#define OVL15_CARD_READ_STATES_H

#include "common.h"

/* Plain card-read scene's frame step (ovl_15:801301A0). After menu lifecycle
 * initialization, call once per frame while this overlay is resident. Uses
 * shared gOvl15MenuState stages 0..19 to poll/choose a card, scan three save
 * slots, read and verify the chosen save, then restore its persistent image.
 * I/O states keep polling until complete. Confirmation/prompts update menu
 * choices and timers; successful restore requests exit with value 1, cancellation
 * with -1. Other stages monitor card changes, alternating gOvl15MonitoredCard
 * when selected-card is 2. Drawing appends to the active render context.
 *
 * No arguments/return value. Do not step the write or transfer scene in the
 * same frame: they share all menu/card state. The transfer scene's separate
 * public frame entry is documented in ovl15_card_read_sprite_states.h; both
 * complete handler sets now compile together in modules/card_read_states.c.
 * Internal stages retain linkage but are not supported external entry points. */
void Ovl15StepCardReadMenu(void);

#endif
