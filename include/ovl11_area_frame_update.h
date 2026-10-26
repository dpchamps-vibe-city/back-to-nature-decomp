#ifndef OVL11_AREA_FRAME_UPDATE_H
#define OVL11_AREA_FRAME_UPDATE_H

#include "common.h"

/*
 * Area frame-phase callback (slot 2 of the overlay phase table). The table
 * dispatcher supplies its base as an unused extra argument; retain this
 * entry's existing no-parameter C contract.
 *
 * Work dispatch 0 runs HUD visibility policy, eligible simulation updates,
 * area-mode services and CD-audio polling. Dispatch 3 updates/applies the
 * camera and draws scene geometry unless hidden. Dispatch 1/2 do no work
 * before the final control-flag transitions. Flags 0x18000 reseed the shared
 * RNG to 1 on entry. Other state flags gate updates and rendering inside
 * the pipeline. Uses shared work areas, packets and scratchpad stack slots;
 * not reentrant. Call in the engine's established frame/phase order.
 */
void Ovl11RunAreaFramePhase(void);

#endif
