#include "common.h"
#include "game_types.h"
#include "ovl11_selection_list_panel.h"
#include "ovl11_value_panels.h"

s32 ovl_11_func_8011D934(StructD548 x);

s32 ovl_11_func_8011A7D8(void) {
    s16 y;

    y = (ovl_11_func_8011D934(*(StructD548 *)gOvl11PrimarySelectionRecord) + 1) * 14 + 20;
    Ovl11DrawCompactValuePanel();
    Ovl11DrawSelectionListPanel((s32 *)gOvl11PrimarySelectionRecord, 0x10, 0x10);
    Ovl11DrawQuantityPromptRows(gOvl11QuantityPrompt, gOvl11QuantityPrompt.unk8,
                       gOvl11QuantityPrompt.unk8 * gOvl11QuantityPrompt.unkA, 0x10, y);
    return 0;
}
