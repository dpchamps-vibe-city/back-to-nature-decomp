#include "common.h"

void ovl_11_func_800F5404(void);
void ovl_11_func_800F5944(void);
void ovl_11_func_800F4A58(void);

void ovl_11_func_800F3FB4(void) {
    if (!(D_8006C848 & 0x4000)) {
        ovl_11_func_800F5404();
        ovl_11_func_800F5944();
        ovl_11_func_800F4A58();
    }
}
