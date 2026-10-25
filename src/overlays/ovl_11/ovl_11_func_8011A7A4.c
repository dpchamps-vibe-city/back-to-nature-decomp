#include "common.h"
#include "game_types.h"

void ovl_11_func_8011CBC8(void);

void ovl_11_func_8011D474(s32 *arg0, s16 arg1, s16 arg2);

s32 ovl_11_func_8011A7A4(void) {
    ovl_11_func_8011CBC8();
    ovl_11_func_8011D474(((s32)(&gOvl11PrimarySelectionRecord)), 0x10, 0x10);
    return 0;
}
