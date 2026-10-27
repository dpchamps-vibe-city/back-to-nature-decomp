#ifndef OVL11_ACTIVE_RECORD_SLOT_DRAIN_H
#define OVL11_ACTIVE_RECORD_SLOT_DRAIN_H

#include "common.h"

/* ovl_11:800DE3A0. Requires initialized record-slot/object-program services.
 * If flag 2 is exactly 1, select object-program release and service phase 2.
 * Post record code 0x12D with value zero, process the pending condition request,
 * and service the active slot. If flag 2 is still exactly 1, keep servicing
 * until the object program reaches state 1 (then service once more) or flag 2
 * becomes zero. There is no timeout: blocked service can wait forever.
 * No result; mutates queue, program and condition-request state. Not reentrant.
 * Used before the dependent action in CD734, not a frame callback. */
void Ovl11DrainActiveRecordSlot(void);

#endif
