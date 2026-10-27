/* Read-only interaction ID service. The adjacent mapper, record query and
 * reset callback are different complete operations, not evidence of one TU.
 * Membership/boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_interaction_state_access.h"

s32 Ovl11GetInteractionId(void) {
    return gOvl11InteractionId;
}
