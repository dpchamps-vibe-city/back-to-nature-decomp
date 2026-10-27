#ifndef OVL11_SCENE_MODE_COMMAND_H
#define OVL11_SCENE_MODE_COMMAND_H

#include "common.h"

/* Post scene mode 12/16/13/14/15/1F for selector 0/1/2/3/4/5 respectively
 * (mode codes are hexadecimal). Always clear shared work word +448C first.
 * Selector 1 also clears bytes 19..1B of both render contexts; selector 5
 * sets work +5234 bit 400. Other selectors leave the current mode unchanged,
 * but still clear +448C. Other flag bits and context bytes are preserved.
 *
 * Returns 1 immediately for EVERY selector, valid or not. No loading, drawing
 * or completion wait occurs here; shared work/render contexts must exist.
 */
s32 Ovl11SetSceneModeCommand(s16 mode);

#endif
