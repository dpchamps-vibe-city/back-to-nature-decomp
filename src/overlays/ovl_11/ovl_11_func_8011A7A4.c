#include "common.h"
#include "game_types.h"
#include "ovl11_selection_list_panel.h"
#include "ovl11_value_panels.h"

s32 ovl_11_func_8011A7A4(void) {
    Ovl11DrawCompactValuePanel();
    Ovl11DrawSelectionListPanel(((s32)(&gOvl11PrimarySelectionRecord)), 0x10, 0x10);
    return 0;
}
