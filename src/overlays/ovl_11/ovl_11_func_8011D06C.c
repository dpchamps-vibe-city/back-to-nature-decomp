#include "common.h"

extern s32 gSpendableBalance;

s32 ovl_11_func_8011D06C(s32 arg0) {
    return gSpendableBalance >= arg0;
}
