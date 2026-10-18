#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

s32 ovl_11_func_800DF4F0(UnkStruct800DF4F0 *arg0);
Ovl11DF72CPair *ovl_11_func_800D08FC(s32 arg0);
s32 *ovl_11_func_800D0C34(void);
s32 ovl_11_func_800D0D7C(u8 *arg0);
void ovl_11_func_800D0DB0(Ovl11DF72CPair *arg0, s16 arg1, s16 arg2);
void ovl_11_func_800D049C(Ovl11DF72CResetView *arg0);
void ovl_11_func_800D12A0(s16 arg0);

s32 ovl_11_func_800DF72C(UnkStruct800DF4F0 *arg0) {
    Ovl11DF72CPair *temp_v0;
    s16 temp_s0;
    s32 *temp_v0_2;
    char *far_base;

    if (ovl_11_func_800DF4F0(arg0) == 0) {
        return -1;
    }
    if (arg0->unk0 != 0x162) {
        return -1;
    }
    if (*(u16 *)((u8 *)arg0 + 0xB2) != 0) {
        return -1;
    }
    if (*(u16 *)((u8 *)arg0 + 0xAE) != 0) {
        return -1;
    }
    temp_v0 = ovl_11_func_800D08FC(1);
    if (temp_v0 == NULL) {
        return -1;
    }
    temp_v0_2 = ovl_11_func_800D0C34();
    if (temp_v0_2 == NULL) {
        return -1;
    }
    temp_s0 = ovl_11_func_800D0D7C((u8 *)temp_v0_2);
    ovl_11_func_800D0DB0(temp_v0, temp_s0, ovl_11_func_800D0D7C((u8 *)arg0));
    *(s32 *)((u8 *)temp_v0_2 + 0x34) |= 0x02000000;
    *(u16 *)((u8 *)arg0 + 0xB2) = 1;
    ovl_11_func_800D049C((Ovl11DF72CResetView *)arg0);
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == *(s16 *)((u8 *)arg0 + 0x30)) {
        ovl_11_func_800D12A0(9);
    }
    return 0;
}
