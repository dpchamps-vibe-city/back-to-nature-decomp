#ifndef OVL11_MENU_FRAME_UPDATE_H
#define OVL11_MENU_FRAME_UPDATE_H

#include "common.h"

/* Service resident interaction/menu panels once in their original frame
 * order, after incrementing the shared mod-120 sprite-animation clock.
 * Controllers can read pad input, change menu/message state and submit draw
 * packets. Requires initialized area/UI state and an active render context;
 * this is not a reset, and must not be called twice for one logical tick. */
void Ovl11UpdateMenuFrame(void);

#endif
