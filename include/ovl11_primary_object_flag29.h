#ifndef OVL11_PRIMARY_OBJECT_FLAG29_H
#define OVL11_PRIMARY_OBJECT_FLAG29_H

#include "common.h"

/* Return 1 when primary-object flags bit 29 (0x20000000) is clear, else 0.
 * The record-movement stage transition uses this guard. Read-only; the bit's
 * broader gameplay meaning is not established.
 */
s32 Ovl11IsPrimaryObjectFlag29Clear(void);

#endif
