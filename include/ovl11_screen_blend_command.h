#ifndef OVL11_SCREEN_BLEND_COMMAND_H
#define OVL11_SCREEN_BLEND_COMMAND_H

#include "common.h"

/* ovl_11:0x800E7BA8. Enqueue a semitransparent 320x240 rectangle at (0,0).
 * Zero selects active ordering-table word 0; any other layer selects word 5.
 * Red/blue contribute their low eight bits. Green contributes sixteen bits
 * before the shift, so its upper byte can overlap red; inputs are not clamped.
 * Uses the current GPU blend mode, consumes packet-work space and returns 1.
 * The active render context, ordering table and packet cursor must be ready;
 * packet storage must remain live until the GPU consumes it. No GPU sync.
 */
s32 Ovl11DrawScreenBlendCommand(s16 layer, s16 red, s32 green, s32 blue);

#endif
