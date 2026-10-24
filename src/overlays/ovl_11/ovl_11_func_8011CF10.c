#include "common.h"

extern s32 D_8005180C;

void func_8001ABF0(u16 *dst, u16 *src);

void ovl_11_func_8011CF10(u16 *dst) {
    func_8001ABF0(dst, (u16 *)(D_80054BC0[0] + (s32)&D_8005180C));
}
