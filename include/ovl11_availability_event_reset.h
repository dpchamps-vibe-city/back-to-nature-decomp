#ifndef OVL11_AVAILABILITY_EVENT_RESET_H
#define OVL11_AVAILABILITY_EVENT_RESET_H

#include "common.h"

/* If the shared +44D4 availability selector is nonzero, draw Rand(100).
 * On a result below 31, clear that selector, reset the alternate area grid,
 * reposition affected records through the collision-reset service, invoke two
 * record cleanup services, clear status bit 0x2000 and game flags 0x39/0x3E.
 * The scene-transition driver invokes this for modes 3/4. No random value is
 * consumed when the selector is zero. No result; all effects are shared state.
 * The gameplay meaning of this availability event is not established. */
void Ovl11TryClearAvailabilityEvent(void);

#endif
