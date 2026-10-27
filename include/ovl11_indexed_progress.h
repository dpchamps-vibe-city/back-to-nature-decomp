#ifndef OVL11_INDEXED_PROGRESS_H
#define OVL11_INDEXED_PROGRESS_H

#include "game_types.h"

/* Unless flags (+36) bit 8 suppresses it, add delta to progress slot
 * (value-1)/5 at +40, saturating only above 65535. Division truncates toward
 * zero; the index must be within the object's 34-halfword array. A negative
 * sum is NOT clamped: the store narrows it modulo 65536. The s32 sum must
 * not overflow. No other fields are changed. */
void Ovl11AddIndexedProgress(Ovl11CD5BCObject *object, s32 delta);

#endif
