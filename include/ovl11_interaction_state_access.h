#ifndef OVL11_INTERACTION_STATE_ACCESS_H
#define OVL11_INTERACTION_STATE_ACCESS_H

#include "common.h"

/* Return the raw shared interaction ID without stepping or resetting it.
 * The dispatcher treats it as an update-table selector; no bounds check here. */
s32 Ovl11GetInteractionId(void);

/* Retail reset callback: ID, step and draw mode -> 0; quantity -> 1;
 * the neutral word at ovl_11:80128210 -> 1. Returns 1 unconditionally.
 * Does NOT clear the resume step, selection rows, callback tables, requests
 * or balances. Shared interaction state must be initialized. */
s32 Ovl11ResetInteractionState(void);

#endif
