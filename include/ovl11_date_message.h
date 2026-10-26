#ifndef OVL11_DATE_MESSAGE_H
#define OVL11_DATE_MESSAGE_H

#include "common.h"

/* Read season/day from initialized engine work and classify
 * (season*30 + day + 20) % 120 against thirteen signed thresholds.
 * Return 0x1BD + first matching interval index (previous < day <= current),
 * or 0x1BD when none matches. The first lower bound is -1. Caller C7AC0
 * uses the result in message archive 2. No globals or output buffers change. */
s32 Ovl11GetDateMessageEntry(void);

#endif
