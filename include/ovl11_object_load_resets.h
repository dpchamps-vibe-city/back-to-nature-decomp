#ifndef OVL11_OBJECT_LOAD_RESETS_H
#define OVL11_OBJECT_LOAD_RESETS_H

#include "common.h"

/* Two independent synchronous reset services; ovl_11 must be resident and
 * the shared event/work storage must be initialized. Neither allocates,
 * releases sprites, changes the archive cursor or restores saved state. */

/* Clear bits 0x1800 of the +0x1E flag halfword in all 37 event records
 * (stride 0x1D4), preserving every other field and flag. The object loader
 * calls this on release. Original identity: ovl_11:800E549C. */
void Ovl11ClearEventRecordLoadFlags(void);

/* Set the ten signed halfwords at shared work +443A..444C to -1, as five
 * pairs. Called on load completion/release and sprite-rotation reset.
 * The adjacent +4438 halfword is NOT reset here. The individual pair roles
 * are not established. Original identity: ovl_11:800E54C8. */
void Ovl11ResetObjectWorkPairs(void);

#endif
