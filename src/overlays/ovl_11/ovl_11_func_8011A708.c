#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

void ovl_11_func_8011CBC8(void);
void ovl_11_func_8011CD2C(Ovl11D8012D538 s, s16 arg3, s32 arg4, s16 arg5, s16 arg6);

s32 ovl_11_func_8011A708(void) {
    ovl_11_func_8011CBC8();
    ovl_11_func_8011CD2C(gOvl11QuantityPrompt, gOvl11QuantityPrompt.unk8, gOvl11QuantityPrompt.unk8 * gOvl11QuantityPrompt.unkA, 0x10, 0x10);
    return 0;
}
