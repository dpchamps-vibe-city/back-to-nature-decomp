#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_value_panels.h"

s32 ovl_11_func_8011A708(void) {
    Ovl11DrawCompactValuePanel();
    Ovl11DrawQuantityPromptRows(gOvl11QuantityPrompt, gOvl11QuantityPrompt.unk8, gOvl11QuantityPrompt.unk8 * gOvl11QuantityPrompt.unkA, 0x10, 0x10);
    return 0;
}
