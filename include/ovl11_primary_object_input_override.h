#ifndef OVL11_PRIMARY_OBJECT_INPUT_OVERRIDE_H
#define OVL11_PRIMARY_OBJECT_INPUT_OVERRIDE_H

#include "common.h"

/* Begin only if primary-object flags 0x40000000 and 0x8 are both clear.
 * Store signed ticks at +2E, save direction +38 to +30, set flags 0xA,
 * enter state 2 with animation selector direction+1, and clear 0x20000000.
 * Ticks are qualifying primary-object updates, not clock minutes. The updater
 * decrements +2E and eventually clears flags 0xA; this call does not tick it.
 * Direction must be 0..3. Negative ticks are stored unchanged, not rejected. */
void Ovl11BeginPrimaryObjectInputOverride(s16 ticks);

/* If record flag 0x8 is set, clear the shared pressed-button mask and replace
 * the held mask with one d-pad bit indexed by signed saved direction at +30.
 * Otherwise leave both masks unchanged. No record field is modified, no real
 * pad input is sampled, and no timer is advanced. Requires +30 and +6C fields;
 * normal callers pass the primary object initialized by the begin operation. */
void Ovl11ApplyPrimaryObjectInputOverride(void *object);

#endif
