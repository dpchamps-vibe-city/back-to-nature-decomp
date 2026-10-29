#ifndef OVL11_PROMPT_OPTION_ROWS_H
#define OVL11_PROMPT_OPTION_ROWS_H

#include "common.h"

/* Build the shared primary and secondary 0x60-byte option records for the
 * two-stage purchase interaction. No arguments; returns 1 (handled).
 * Refreshes row availability from progress flags, installs bank-1 text
 * pointers and row values/types, chooses an occupied selection with Rand,
 * and clears gOvl11InteractionDrawMode. It prepares records, not GPU packets;
 * the selection renderer/input services consume them afterward. Shared state
 * makes the operation non-reentrant; caller must enter the interaction first.
 * Other handlers in this module are installed through the raw callback table.
 */
s32 Ovl11FillPrimaryAndSecondaryPromptRows(void);

#endif
