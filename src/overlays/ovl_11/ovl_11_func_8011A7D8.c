#include "common.h"
#include "game_types.h"

s32 ovl_11_func_8011D934(StructD548 x);
void ovl_11_func_8011CBC8(void);
void ovl_11_func_8011D474(s32 *arg0, s16 arg1, s16 arg2);
void ovl_11_func_8011CD2C(Ovl11D8012D538 s, s16 arg3, s32 arg4, s16 arg5, s16 arg6);

s32 ovl_11_func_8011A7D8(void) {
    s16 y;

    y = (ovl_11_func_8011D934(*(StructD548 *)D_8012D548) + 1) * 14 + 20;
    ovl_11_func_8011CBC8();
    ovl_11_func_8011D474((s32 *)D_8012D548, 0x10, 0x10);
    ovl_11_func_8011CD2C(D_8012D538, D_8012D538.unk8,
                       D_8012D538.unk8 * D_8012D538.unkA, 0x10, y);
    return 0;
}
