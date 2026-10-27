#ifndef OVL11_PROGRESS_TRIGGER_LATCH_H
#define OVL11_PROGRESS_TRIGGER_LATCH_H

#include "common.h"

/* If gProgressStateFlags bit 10 is set and bit 26 is clear, set bit 26 and
 * return 1. Otherwise leave all state unchanged and return 0. Bit 10 is not
 * cleared. No request is installed here: the interaction dispatcher does so
 * after success. Other bits are ignored and preserved; no reset is performed. */
s32 Ovl11TryLatchProgressTrigger(void);

#endif
