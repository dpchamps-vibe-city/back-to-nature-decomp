#ifndef OVL10_CARD_DIAGNOSTICS_H
#define OVL10_CARD_DIAGNOSTICS_H

#include "common.h"

/* Run the resident PlayStation/PDA command-menu scene until Select requests
 * exit, then request scene 0 and return 0. This is a blocking frame loop:
 * it polls both pads, services input/command status and waits on GPU/VSync.
 * L2/L1 select a command, Start submits it, and Select exits. Input comes
 * from pad 0's newly pressed mask, not its held/auto-repeat mask.
 * Pad and Mcx communication must already be initialized. Overlay data must
 * be freshly initialized, in particular gOvl10MenuExitRequested == 0;
 * re-entering with the exit latch set skips the menu.
 *
 * Installs two 640x240 draw/display environments and the SDK debug font,
 * leaves those environments installed, and initializes only the first six
 * device-transfer bytes (not the complete buffers or repeat/cursor state).
 * Command editors share result text and transfer storage and are not
 * reentrant. SDK command acceptance/completion is displayed without retrying
 * a rejected submission. The original buffer-length and cursor contracts
 * are unchecked; this is a diagnostic interface, not a validated card API. */
s32 Ovl10RunPdaCommandMenu(void);

#endif
