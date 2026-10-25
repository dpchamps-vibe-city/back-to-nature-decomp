#include "common.h"
#include "game_types.h"

s32 ovl_11_func_8011D934(StructD548 x);
void ovl_11_func_8011CBC8(void);
void ovl_11_func_8011D474(u8 *arg0, s16 arg1, s16 arg2);

s32 ovl_11_func_8011A8D0(void) {
    s16 y;

    y = (ovl_11_func_8011D934(*(StructD548 *)gOvl11PrimarySelectionRecord) + 1) * 14 + 20;
    ovl_11_func_8011CBC8();
    ovl_11_func_8011D474(gOvl11SecondarySelectionRecord, 0x10, y);
    ovl_11_func_8011D474(gOvl11PrimarySelectionRecord, 0x10, 0x10);
    return 0;
}
