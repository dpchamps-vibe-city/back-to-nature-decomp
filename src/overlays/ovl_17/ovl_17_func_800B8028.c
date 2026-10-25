#include "common.h"
#include "message_window.h"
#include "ovl17_timer_hud.h"

void ovl_17_func_800B8028(void) {
    UpdateMessageWindow();
    D_800BB4F8[D_800BD84C]();
    Ovl17UpdateTimerHud(1);
}
