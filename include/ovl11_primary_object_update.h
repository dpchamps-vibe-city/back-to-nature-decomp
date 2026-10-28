#ifndef OVL11_PRIMARY_OBJECT_UPDATE_H
#define OVL11_PRIMARY_OBJECT_UPDATE_H

/* Service the primary object in D_8006C838 once per area update.
 * Requires initialized object, input and collision/action services. Selects
 * live held/pressed buttons or stored substitute masks, dispatches actions,
 * advances eligible timers/effects, and samples position/direction history.
 * A nonzero action-dispatch result clears the shared u16 footprint-power
 * accumulator. The stored dispatch result is narrowed to a signed halfword;
 * the activation service distinguishes result 2 from other results.
 * Its adjacent surface helper probes a facing-offset COPY of object position
 * and caches the polygon word in gOvl11PrimarySurfaceFlags, substituting 0x8000
 * when the word has no upper-half bits; it does not move the object.
 * Clock/fade/object guards can skip work; this is not an unconditional tick.
 * No return value. State and counter units are area-update calls, not ms.
 */
void Ovl11UpdatePrimaryObject(void);

#endif
