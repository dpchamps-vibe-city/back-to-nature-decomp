#ifndef OVL11_PRIMARY_OBJECT_WAIT_STEPS_H
#define OVL11_PRIMARY_OBJECT_WAIT_STEPS_H

#include "common.h"

/* Resident ovl_11, initialized primary-object state, and a writable step
 * pointer are required. The caller owns an absolute interaction-step index;
 * baseStep selects this operation's two phases. Values outside baseStep and
 * baseStep+1 return 0 without writing the index. Phase 0 submits once, writes
 * baseStep+1 and returns 0. Phase 1 polls on each invocation, returns 1 when
 * complete, and leaves the index at baseStep+1 (the caller advances it).
 * These functions do not update the primary object's animation themselves. */

/* Submit itemId through the direct-ID request service, ignoring its return
 * value: even a rejected submission advances to polling and is not retried.
 * Completion requires (primary flags & 0x34008000) == 0x14008000. */
s32 Ovl11StepPrimaryObjectItemRequest(s32 *step, s32 baseStep, s16 itemId);

/* Submit the three halfwords {0xA4, requestValue, 1}. The second halfword's
 * gameplay interpretation is not established. Completion tests flag 0x04000000. */
s32 Ovl11StepPrimaryObjectA4Request(s32 *step, s32 baseStep, s16 requestValue);

#endif
