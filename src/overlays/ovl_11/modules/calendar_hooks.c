/* Calendar-panel lifecycle hooks (ovl_11:800F6638–800F6650).
 * The retail implementation leaves opening/closing to calendar_panel; these
 * extension points have no state or side effects. Keep their callable ABI.
 * Membership and evidence-limited boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_calendar_hooks.h"

void Ovl11CalendarOpenHook(void) {
}

void Ovl11CalendarCloseHook(void) {
}

s32 Ovl11CalendarCloseHookEnabled(void) {
    return 1;
}
