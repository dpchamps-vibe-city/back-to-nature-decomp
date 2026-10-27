#ifndef OVL11_INTERACTION_VOLUME_ACTIVATION_H
#define OVL11_INTERACTION_VOLUME_ACTIVATION_H

#include "common.h"
#include "game_types.h"

/* Probe the loaded area's volumes at object+100 (X/Z), applying probe effects.
 * Uses object flags, facing, pressed button 0x40 and shared work/game flags.
 * Result: 2 permits normal motion; 0 consumes the frame without activation;
 * 1 reports activation to the area-mode controller. Messages use archive 2;
 * input overrides last 5, 10 or 20 update calls, not milliseconds.
 *
 * gOvl11SelectedInteractionVolume borrows the matched record+10 view, or null
 * on a completed probe that misses. Early guards leave the old pointer intact.
 * The area-mode consumer needs the loaded volume storage to remain alive.
 * No new allocation, clamping or record validation is performed here.
 */
s32 Ovl11ActivateInteractionVolume(Ovl11C7010Object *object);

#endif
