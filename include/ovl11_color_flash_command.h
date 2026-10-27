#ifndef OVL11_COLOR_FLASH_COMMAND_H
#define OVL11_COLOR_FLASH_COMMAND_H

#include "common.h"

/* ovl_11:0x800EE240. Four consecutive value slots hold timer, red, green,
 * blue; firstSlot must leave room for all four in the shared slot bank.
 * Nonzero resetDelay only seeds timer = -200 - 5*Rand(80), leaving RGB alone.
 * Otherwise advance timer by five: at 0 play SFX 46; at 300 copy the private
 * RGB seed, raise two independently chosen (possibly identical) channels to
 * 255 and subtract Rand(32) from each; at 330 play SFX 47; at >=360 clear red
 * and seed another delay. Timer units are not milliseconds.
 * While red != 0, enqueue a 320x240 additive semitransparent rectangle in OT
 * word 5, then subtract 15 from each RGB slot, flooring at zero. Red zero
 * suppresses BOTH drawing and decay, even if green/blue remain nonzero.
 * Color packing reads red/blue low bytes and green's low HALFWORD, which may
 * overlap red bits for externally supplied slot values. The draw installs
 * additive GPU blend mode without restoring the previous mode. Requires
 * live render context, initialized packet work and retained packet storage.
 * Always returns 1; no synchronization or slot-range checks.
 */
s32 Ovl11UpdateColorFlashCommand(s16 resetDelay, s16 firstSlot);

#endif
