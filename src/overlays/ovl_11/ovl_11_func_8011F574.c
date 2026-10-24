#include "common.h"

void ovl_11_func_8011F52C(void);

char *ovl_11_func_8011F574(void) {
    char *base;
    char *start;
    u32 v;
    s16 *flag;

    ovl_11_func_8011F52C();
    flag = &D_80128540;

    /* Keep the work-area origin separate from its upper-half view. */
    start = (char *)&D_8006C838;
    base = start + 0x8000;

    v = *(u16 *)(base + 0x64D2);
    *flag = 1;
    *(u16 *)(base + 0x64D2) = 0;
    D_8012855A = v;
    return base;
}