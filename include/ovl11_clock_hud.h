#ifndef OVL11_CLOCK_HUD_H
#define OVL11_CLOCK_HUD_H

#include "common.h"

/* ovl_11's compact upper-right date/time HUD. Requires initialized render,
 * sprite and text services. Reads zero-based season/day/weekday and hour/
 * minute from work +44BA..44C2; callers supply a valid date and clock time.
 * Writes GPU packets and the private 32-byte text buffer, not the game clock.
 * Non-reentrant: formatting and drawing share engine text/sprite state. */

/* Hide unconditionally. Original identity: ovl_11:800FB5FC. */
void Ovl11ResetClockHud(void);

/* Visibility controls: Enable writes 1 only from zero; Disable clears any
 * nonzero value. Neither draws, clears pixels, or changes the clock.
 * Original identities: ovl_11:800FB608 / 800FB628. */
void Ovl11EnableClockHud(void);
void Ovl11DisableClockHud(void);

/* Call once per rendered frame; a zero enable word does nothing. Draws
 * season, day+1, color-coded weekday and AM/PM time in pixel coordinates.
 * Hours >=12 subtract 12 (no special conversion of zero to 12). The final
 * minute glyph selects 0/5 via (minute % 10)/5; it does not round or tick.
 * Restores the incoming text style. Original identity: ovl_11:800FB648. */
void Ovl11UpdateClockHud(void);

#endif
