#include "common.h"
#include "game_types.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_primary_object_requests.h"

s32 ovl_11_func_800E109C(UnkStruct800E109C *arg0);
void ovl_11_func_800D049C(Ovl11DF72CResetView *arg0);
void ovl_11_func_800D12A0(s16 arg0);
s16 ovl_11_func_800E0E38(UnkStruct800E109C *arg0);

s32 ovl_11_func_800E1404(UnkStruct800E109C *arg0) {
    s16 sp10[3];
    s16 temp_v1;
    s16 var_s1;
    s32 var_v0;
    Ovl11DF84CStateView *temp_v0;
    u8 *far_base;
    u8 *mode_base;
    u8 *state_base;

    if (ovl_11_func_800E109C(arg0) == 0) {
        return -1;
    }
    if (arg0->unk0 != 0x165) {
        return -1;
    }
    if (*(u16 *)((u8 *)arg0 + 0xB2) != 0) {
        return -1;
    }
    if (*(u16 *)((u8 *)arg0 + 0xB4) < 7U) {
        return -1;
    }
    temp_v1 = *(s16 *)((u8 *)arg0 + 0x16);
    var_s1 = 0;
    if (temp_v1 >= 0xC9) {
        var_s1 = 0x93;
        if (*(s32 *)((u8 *)arg0 + 0x34) & 0x40) {
            var_s1 = 0xE7;
        }
    } else if (temp_v1 >= 0x65) {
        var_s1 = 0x92;
    } else if (temp_v1 >= 0) {
        var_s1 = 0x91;
    }
    if (*(s16 *)((u8 *)arg0 + 0xB6) == -1) {
        if (var_s1 != 0) {
            *(u16 *)((u8 *)arg0 + 0xB4) = 0;
            Ovl11SetHalfwordTriplet(sp10, var_s1, 0, 1);
            Ovl11InstallPrimaryObjectRequest((u16 *)sp10);
            ovl_11_func_800D049C((Ovl11DF72CResetView *)arg0);
            goto block_19;
        }
    } else if (var_s1 != 0) {
        state_base = (u8 *)D_8006C838;
        temp_v0 = (Ovl11DF84CStateView *)
            (state_base + (*(s16 *)((u8 *)arg0 + 0xB6) * 0xF8));
        if (temp_v0->pending == 0) {
            *(u16 *)((u8 *)arg0 + 0xB4) = 0;
            temp_v0->pending = (u16)var_s1;
            ovl_11_func_800D049C((Ovl11DF72CResetView *)arg0);
            mode_base = (u8 *)&D_8007AFF0;
            if (*(s16 *)(mode_base + 0x25476) == 1 ||
                *(s16 *)(mode_base + 0x25476) == 5) {
block_19:
                ovl_11_func_800E0E38(arg0);
            } else {
                *(s32 *)((u8 *)arg0 + 0x8C) = 0;
            }
        } else {
            var_s1 = 0;
        }
        if (var_s1 != 0) {
            far_base = (u8 *)&D_8007AFF0;
            if (*(s16 *)(far_base + 0x25476) == *(s16 *)((u8 *)arg0 + 0x30)) {
                ovl_11_func_800D12A0(0xB);
            }
        }
    }
    var_v0 = 0;
    if (var_s1 == 0) {
        var_v0 = -1;
    }
    return var_v0;
}
