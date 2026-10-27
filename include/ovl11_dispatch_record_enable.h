#ifndef OVL11_DISPATCH_RECORD_ENABLE_H
#define OVL11_DISPATCH_RECORD_ENABLE_H

#include "common.h"

/* Find the first signed-halfword kind in the thirteen 0x18-byte records
 * selected by shared-work+0xDDB4. enabled == 1 sets flag bit 0; EVERY other
 * value clears it, preserving all other halfword flag bits. No match is a
 * no-op. The list must be initialized; this neither allocates a record nor
 * updates the other dispatch lists serviced by record_pool_refresh.
 */
void Ovl11SetDispatchRecordEnabled(s16 kind, s32 enabled);

#endif
