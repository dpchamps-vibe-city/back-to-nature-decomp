#include "common.h"
#include "game_types.h"
#include "ovl11_selection_list_panel.h"

s32 ovl_11_func_8011D934(StructD548 x);
void ovl_11_func_8011CBC8(void);
void ovl_11_func_8011CD2C(Ovl11D8012D538 s, s16 arg3, s32 arg4, s16 arg5, s16 arg6);

s32 ovl_11_func_8011A7D8(void) {
    s16 y;

    y = (ovl_11_func_8011D934(*(StructD548 *)gOvl11PrimarySelectionRecord) + 1) * 14 + 20;
    ovl_11_func_8011CBC8();
    Ovl11DrawSelectionListPanel((s32 *)gOvl11PrimarySelectionRecord, 0x10, 0x10);
    ovl_11_func_8011CD2C(gOvl11QuantityPrompt, gOvl11QuantityPrompt.unk8,
                       gOvl11QuantityPrompt.unk8 * gOvl11QuantityPrompt.unkA, 0x10, y);
    return 0;
}
