#ifndef OVL11_AREA_FRAME_UPDATE_DISPATCH_H
#define OVL11_AREA_FRAME_UPDATE_DISPATCH_H
#include "common.h"

/* Unless shared state +10 bit 2000 inhibits updates, service the four record
 * pool updaters and pending stage mode in fixed order. In stage 1, request a
 * code15B record during hours 18..22 (at hour 18 only from minute 10), gated
 * by +44F8 mask 01800000 == 00800000. Finally start and consume a nonzero
 * pending CD track only while audio is idle and +CC is zero. Requires loaded
 * area/pools and initialized audio. No elapsed-time scaling; call per frame. */
void Ovl11RunAreaFrameUpdates(void);
#endif
