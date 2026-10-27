/* Retail interaction reset callback, reached through the dispatcher's reset
 * pointer. Reset only the state named here; resume/selection/request state
 * belongs to its other services. Membership/boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_interaction_state_access.h"

s32 Ovl11ResetInteractionState(void) {
    gOvl11InteractionId = 0;
    D_80128210 = 1;
    gOvl11InteractionStep = 0;
    gOvl11InteractionDrawMode = 0;
    gOvl11QuantityPromptCount = 1;
    return 1;
}
