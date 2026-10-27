#ifndef OVL11_ACTION_RESULT_REQUEST_H
#define OVL11_ACTION_RESULT_REQUEST_H

#include "common.h"

/* Overlay 11. Resolve an action ID to a signed result (clients use archive-2
 * message entries), or request rotation for that ID. direction is passed
 * unchanged to the rotation service; transitions require 0..3 and a valid
 * registered source, as described in sprite_rotation.h.
 *
 * forceRotation == 0: the LAST matching non--1 ID in the five override slots
 * wins. Otherwise query the guard service. A guard result is replaced with
 * -1 unless the sprite is idle, but only when resultOut is non-NULL.
 * forceRotation != 0 skips guards and ignores matching override slots.
 *
 * Special case: with NO matching override, ID 0x20 and game flag 0x76 == 1
 * write 0x209 even in forced mode. resultOut MUST be non-NULL on that path.
 * Flag 0x04000000 of the shared work flags suppresses only guard results,
 * not slot results or this special case.
 *
 * Result paths return 0, not a success boolean. Other paths return the
 * rotation service's handled result (0/1) and leave *resultOut unchanged.
 * The output pointer is otherwise optional. Guard lookup also mutates its
 * shared selector. This service is not reentrant and does not clear slots. */
s32 Ovl11ResolveActionResultOrRotate(s16 actionId, u16 direction,
                                    s32 *resultOut, s32 forceRotation);

#endif
