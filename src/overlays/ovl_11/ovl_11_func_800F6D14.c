#include "common.h"

void ovl_11_func_800F7010(s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800F8CF8(s32 *arg0, s16 arg1, s16 arg2, s16 arg3);
void ovl_11_func_800F6A24(u16 arg0);
void ovl_11_func_800F6C78(s32 *arg0, u16 arg1, s16 arg2, s16 arg3);

void ovl_11_func_800F6D14(s32 *arg0, u16 arg1, s16 arg2, s16 arg3) {
    s32 i;
    u16 id;

    id = arg1 & 0xFFFF;
    if (id == 0) {
        ovl_11_func_800F7010((s32)(arg0 + 1), arg2, arg3);
        return;
    }
    for (i = 0; i < 0x48; i++) {
        if (D_80129648_textures[i].id == id) {
            D_80129648_textures[i].active = 1;
            ovl_11_func_800F8CF8(arg0, i, arg2, arg3);
            return;
        }
    }
    for (i = 0x47; i >= 0; i--) {
        if (D_80129648_textures[i].active == 0) {
            D_80129648_textures[i].id = 0;
            ovl_11_func_800F6A24(id);
            ovl_11_func_800F6C78(arg0, id, arg2, arg3);
            return;
        }
    }
}
