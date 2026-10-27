#ifndef OVL11_QUANTITY_PROMPT_SETUP_H
#define OVL11_QUANTITY_PROMPT_SETUP_H

#include "common.h"
#include "game_types.h"

/* Update a valid caller-owned 12-byte descriptor while ovl_11 is resident.
 * Always store primaryTextAddress and signed unitValue. A nonzero quantity
 * replaces the current quantity; zero preserves it. With preserveSecondaryText
 * ==0, set secondaryTextAddress to the default prompt text anchor plus the
 * current text-bank offset; otherwise preserve it. Text addresses are raw
 * words in this ABI and their storage must remain valid until drawing.
 * No input service, quantity validation, allocation, sound or drawing. */
void Ovl11SetupQuantityPrompt(Ovl11QuantityPromptView *prompt, s32 primaryTextAddress,
                            s16 preserveSecondaryText, s16 unitValue, s16 quantity);

#endif
