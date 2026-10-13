#include "common.h"

void ovl_11_func_800F8F3C(s32 *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4);
void ovl_11_func_800F6A24(u16 arg0);

void ovl_11_func_800F6E38(s32 *arg0, u16 arg1, s16 arg2, s16 arg3, s32 arg4) {
    s32 i;
    u16 id;

    id = arg1 & 0xFFFF;
    if (id != 0) {
        for (i = 0; i < 0x48; i++) {
            if (D_80129648_textures[i].id == id) {
                D_80129648_textures[i].active = 1;
                ovl_11_func_800F8F3C(arg0, i, arg2, arg3, arg4);
                return;
            }
        }
        for (i = 0x47; i >= 0; i--) {
            if (D_80129648_textures[i].active == 0) {
                D_80129648_textures[i].id = 0;
                ovl_11_func_800F6A24(id);
                ovl_11_func_800F6E38(arg0, id, arg2, arg3, arg4);
                return;
            }
        }
    }
}
