#include "common.h"
#include "game_types.h"

typedef struct {
    char pad0[0x36];
    u16 flags;
    char pad38[0x40 - 0x38];
    u16 vals[(0x84 - 0x40) / 2];
    s16 value;
} M2C_583b0b68ff5a_UnkStruct800CD5BC;

s32 ovl_11_func_800C9D64(void);
void ovl_11_func_800CD5BC(M2C_583b0b68ff5a_UnkStruct800CD5BC *arg0, s32 arg1);
void ovl_11_func_800C5048(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800C4C94(Ovl11C7010Object *arg0) {
    Ovl11CE034Position pos;
    s16 col;
    s16 row;
    s16 temp_a0;
    s32 temp_s1;
    s32 temp_v1;
    s32 var_s0;
    s32 temp_v0;

    temp_v0 = ovl_11_func_800C9D64();
    if (temp_v0 == 2) {
        ovl_11_func_800CD5BC((M2C_583b0b68ff5a_UnkStruct800CD5BC *)arg0, 0x28);
        return 0;
    }
    pos = *(Ovl11CE034Position *)((u8 *)arg0 + 0x100);
    temp_s1 = (s16)((s16)*(u16 *)((u8 *)arg0 + 0x18) / 20);
    var_s0 = D_801231A8[temp_s1];
    temp_a0 = *(s16 *)((u8 *)arg0 + 0x88);
    temp_s1 -= *(s16 *)((u8 *)arg0 + 0x86);
    temp_v1 = temp_a0 - var_s0;
    if (temp_a0 < var_s0) {
        var_s0 = temp_a0 - 1;
    }
    *(s16 *)((u8 *)arg0 + 0x88) = temp_v1;
    if ((s16)temp_v1 < 0) {
        *(s16 *)((u8 *)arg0 + 0x88) = 0;
    }
    D_80123234[temp_v0](&pos, &col, &row);
    ovl_11_func_800C5048(1, (s16)temp_v0, col, row, temp_s1, var_s0);
    ovl_11_func_800C70CC(arg0, 0x68, 0, -2);
    return 1;
}
