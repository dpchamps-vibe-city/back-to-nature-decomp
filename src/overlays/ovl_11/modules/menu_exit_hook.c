/* Standalone menu-exit notification wrapper, ovl_11:800F6650–800F6670.
 * No arguments or result. The forwarded service clears work-word bits 26/27
 * and sets status-halfword bit 6; repeated calls are idempotent. This entry
 * has no additional state or retail caller established, so it is not folded
 * into the unrelated calendar hook protocol immediately before it.
 * Membership and evidence-limited boundaries: notes/file-groupings.md. */

#include "common.h"
#include "ovl11_menu_exit_flags.h"

void Ovl11ForwardMenuExitFlags(void) {
    Ovl11PostMenuExitFlags();
}
