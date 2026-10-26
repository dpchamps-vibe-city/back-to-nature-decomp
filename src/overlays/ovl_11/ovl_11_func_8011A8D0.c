#include "common.h"
#include "game_types.h"
#include "ovl11_selection_list_panel.h"
#include "ovl11_value_panels.h"
#include "ovl11_selection_record_access.h"

s32 ovl_11_func_8011A8D0(void) {
    s16 y;

    y = (Ovl11CountPrecedingSelectionEntries(*(StructD548 *)gOvl11PrimarySelectionRecord) + 1) * 14 + 20;
    Ovl11DrawCompactValuePanel();
    Ovl11DrawSelectionListPanel(gOvl11SecondarySelectionRecord, 0x10, y);
    Ovl11DrawSelectionListPanel(gOvl11PrimarySelectionRecord, 0x10, 0x10);
    return 0;
}
