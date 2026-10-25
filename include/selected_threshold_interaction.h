#ifndef SELECTED_THRESHOLD_INTERACTION_H
#define SELECTED_THRESHOLD_INTERACTION_H

#include "common.h"
#include "game_types.h"

/* Service the active selection's confirmation/message sequence once.
 * Uses gOvl11PrimarySelectionRecord (unchecked signed index into its 26-entry
 * table) and gOvl11InteractionStep. Returns 1 only at completion step 11,
 * otherwise 0. The threshold is the signed low halfword of the code lookup;
 * its balance units are not established as currency or a gameplay resource.
 *
 * Confirmation debits the shared balance without saturation, then dispatches
 * selector-specific actions. Also changes interaction draw mode, work flags,
 * message-window state and the shared text service. Non-reentrant: the caller
 * must initialize the interaction service and keep the selection valid until
 * completion. This is an update-call interface, not a blocking operation. */
s32 Ovl11UpdateThresholdInteraction(void);

#endif
