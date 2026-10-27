/* Retail area-setup extension point (ovl_11:800F6630–800F6638).
 * Distinct from the calendar lifecycle hooks that follow it. The area loader
 * retains this out-of-line call even though the implementation does nothing.
 * Evidence-limited service boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_area_setup_hook.h"

void Ovl11AreaSetupHook(void) {
}
