#ifndef OVL11_INTERACTION_RECORD_ACTIONS_H
#define OVL11_INTERACTION_RECORD_ACTIONS_H

#include "common.h"
#include "game_types.h"

/* Service hit category 0x80. A nonzero result reports a handled request;
 * zero does NOT imply no mutation: object state 0x32 can still select response
 * 9 and update its flags/record id. The ordinary path selects response 3 and
 * clears the primary-object request. Requires a valid latched hit record
 * whenever category 0x80 is set; no null check is performed. */
s32 Ovl11HandleInteractionRequest80(Ovl11C8FA8Object *object);

/* For pressed bit 0x40, process the shared hit categories in order: 0x80,
 * 0x400, 0x200, 2, then 0x8020. May install/save a record, change object state,
 * play sound, reset a request or queue an archive-2 message. The 0x400 branch
 * may fall through, and the final response service returns 0 despite effects.
 * Return 1 for a handled branch, otherwise 0; shared latches are not reentrant.
 * The pointer uses the established primary-object interaction view. */
s32 Ovl11DispatchInteractionRecordInput(Ovl11C8FA8Object *object, u16 pressed);

#endif
