#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

typedef struct {
    char pad0[0x36];
    u16 flags;
    char pad38[0x40 - 0x38];
    u16 vals[(0x84 - 0x40) / 2];
    s16 value;
} M2C_583b0b68ff5a_UnkStruct800CD5BC;

s32 ovl_11_func_800C9D64(void);
void ovl_11_func_800C5048(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, u16 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800CD5BC(M2C_583b0b68ff5a_UnkStruct800CD5BC *arg0, s32 arg1);

s32 ovl_11_func_800C4B6C(Ovl11C7010Object *arg0) {
    Ovl11CE034Position pos;
    s16 col;
    s16 row;
    s32 tier;
    s16 value;
    s16 *entry;
    s32 mode;
    s32 result;

    mode = ovl_11_func_800C9D64();
    if (mode != 2) {
        pos = *(Ovl11CE034Position *)((u8 *)arg0 + 0x100);
        tier = (s16)((s16)*(u16 *)((u8 *)arg0 + 0x18) / 20);
        entry = &D_8012319C[tier];
        tier -= *(s16 *)((u8 *)arg0 + 0x86);
        value = *entry;
        D_80123234[mode](&pos, &col, &row);
        ovl_11_func_800C5048(0, (s16)mode, col, row, tier, value);
        ovl_11_func_800C70CC(arg0, 0x68, 0, -2);
        result = 1;
    } else {
        ovl_11_func_800CD5BC((M2C_583b0b68ff5a_UnkStruct800CD5BC *)arg0, 0x28);
        result = 0;
    }
    return result;
}
