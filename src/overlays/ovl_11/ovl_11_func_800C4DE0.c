#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800C9D64(void);
s32 ovl_11_func_800F46A4(Ovl11SpawnEffectRecord *arg0);
void ovl_11_func_800CD5BC(Ovl11CD5BCObject *arg0, s32 arg1);
void ovl_11_func_800C5048(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, u16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800C4DE0(Ovl11C7010Object *arg0) {
    Ovl11CE034Position pos;
    s16 col;
    s16 row;
    s32 tier;
    s16 *entry;
    s16 value;
    s32 mode;
    s32 result;

    mode = ovl_11_func_800C9D64();
    if (mode == 2) {
        if (D_80128C54 & 0x8000) {
            if (ovl_11_func_800F46A4(D_80128C4C_dispatch) != 0) {
                ovl_11_func_800CD5BC((Ovl11CD5BCObject *)arg0, 0x96);
            }
        } else {
            ovl_11_func_800CD5BC((Ovl11CD5BCObject *)arg0, 0x28);
        }
        result = 0;
    } else {
        pos = *(Ovl11CE034Position *)((u8 *)arg0 + 0x100);
        tier = (s16)((s16)*(u16 *)((u8 *)arg0 + 0x18) / 20);
        entry = &D_801231B4[tier];
        tier -= *(s16 *)((u8 *)arg0 + 0x86);
        value = *entry;
        D_80123234[mode](&pos, &col, &row);
        ovl_11_func_800C5048(2, (s16)mode, col, row, tier, value);
        ovl_11_func_800C70CC(arg0, 0x68, 0, -2);
        result = 1;
    }
    return result;
}
