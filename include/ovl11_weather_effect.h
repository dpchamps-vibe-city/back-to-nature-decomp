#ifndef OVL11_WEATHER_EFFECT_H
#define OVL11_WEATHER_EFFECT_H

#include "common.h"

/* ovl_11 must be resident. This effect combines hour-class ambient tint with
 * 40 screen-space falling/splash records. State is shared and non-reentrant;
 * initialize shared sprite data, render context and packet work before drawing.
 * The separate screen_color_ramp.h describes the Q12 interpolation helpers. */

/* Gated scene tick: update/draw particles, retarget tint over 256 permitted
 * updates when the hour class changes, then draw/step the tint. Scene and
 * sprite-work flags can independently suppress these operations. */
void Ovl11UpdateWeatherEffect(void);

/* Select this hour's table triple with a one-update ramp. Does not draw it
 * or immediately replace RGB: the next permitted ramp step applies it. */
void Ovl11SelectCurrentHourTint(void);

/* Clear all particle records and restore the default tint-table pointer.
 * Does NOT reset the current screen tint; call ResetAmbientTintFromTable for
 * that separate operation. The reset hook is empty in the original build. */
void Ovl11ResetWeatherParticles(void);
void Ovl11WeatherResetHook(void);

/* Copy the active table's FIRST triple to persistent RGB and clear the ramp's
 * deltas/counters and retained hour class. This is not current-hour selection. */
void Ovl11ResetAmbientTintFromTable(void);

/* Modes 1..4 select animation 0x19/0x1A and falling/variant flag combinations,
 * install the alternate tint table and respawn every record using Rand.
 * Other values do nothing. Existing flag bits are ORed, not cleared: callers
 * normally reset particles before selecting a mode. Positions/delays are
 * randomized; the otherwise-unused Rand calls are part of the state changes. */
void Ovl11SetWeatherParticleMode(s16 mode);

#endif
