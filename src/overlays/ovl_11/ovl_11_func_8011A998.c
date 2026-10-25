#include "common.h"
#include "game_types.h"
#include "ovl11_selection_list_panel.h"
#include "ovl11_value_panels.h"

s32 ovl_11_func_8011A998(void) {
    Ovl11DrawWideValuePanel();
    Ovl11DrawSelectionListPanel(((s32)(&gOvl11PrimarySelectionRecord)), 0x10, 0x30);
    return 0;
}
