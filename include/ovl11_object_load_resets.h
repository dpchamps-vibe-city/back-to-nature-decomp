#ifndef OVL11_OBJECT_LOAD_RESETS_H
#define OVL11_OBJECT_LOAD_RESETS_H

#include "common.h"

/* Independent synchronous reset services; ovl_11 must be resident and the
 * shared event/work storage must be initialized. None allocates or frees
 * memory, changes the archive cursor or restores saved state. */

/* Unbind all ten entity sprite sources by clearing only each +14 header
 * address (stride 30 hex), leaving their other fields intact. Then clear the
 * parallel spawn-record pool using the initialized work +DDD0 pointer and
 * signed +DDFA count, and set work +4476 to 1 to suppress current-object
 * sprite drawing. This is a reset, not a GPU upload or a memory release.
 * Called before archive loading and on release; no bounds checks or waits.
 * Original identity: ovl_11:800E5230. */
void Ovl11ResetEntitySpriteSources(void);

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
