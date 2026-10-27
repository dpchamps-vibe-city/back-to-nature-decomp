/* Inactivity query for interaction 0x1A (ovl_11:801165C8).
 * The state-command adapter pairs this query with BeginInteraction(0x1A).
 * It reads only the dispatcher ID; it does not advance the interaction.
 */
#include "selected_threshold_interaction.h"

s32 Ovl11IsThresholdInteractionInactive(void) {
    return gOvl11InteractionId != 0x1A;
}
