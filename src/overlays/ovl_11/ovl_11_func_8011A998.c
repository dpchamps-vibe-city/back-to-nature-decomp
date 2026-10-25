#include "common.h"
#include "game_types.h"
#include "ovl11_selection_list_panel.h"

void ovl_11_func_8011CBE8(void);

s32 ovl_11_func_8011A998(void) {
    ovl_11_func_8011CBE8();
    Ovl11DrawSelectionListPanel(((s32)(&gOvl11PrimarySelectionRecord)), 0x10, 0x30);
    return 0;
}
