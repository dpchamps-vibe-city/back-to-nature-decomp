/*
 * One-shot progress trigger, ovl_11:800F3F7C–800F3FB4.
 * The interaction dispatcher uses success to install request 0x97. Other
 * progress-state bits belong to their existing producers/consumers.
 */
#include "common.h"
#include "ovl11_progress_trigger_latch.h"

s32 Ovl11TryLatchProgressTrigger(void) {
    if ((gProgressStateFlags & 0x04000400) == 0x400) {
        gProgressStateFlags |= 0x04000000;
        return 1;
    }
    return 0;
}
