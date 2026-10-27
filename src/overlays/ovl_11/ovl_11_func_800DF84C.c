#include "common.h"
#include "game_types.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_record_effect_threshold.h"

s32 ovl_11_func_800DF4F0(UnkStruct800DF4F0 *arg0);
void ovl_11_func_800D12A0(s16 arg0);

s32 ovl_11_func_800DF84C(UnkStruct800DF4F0 *arg0) {
    s16 sp10[3];
    s16 temp_v1;
    s16 var_s0;
    s32 var_v0;
    Ovl11DF84CStateView *temp_v0_2;
    u8 *far_base;
    u8 *state_base;

    if (ovl_11_func_800DF4F0(arg0) == 0) {
        return -1;
    }
    if (arg0->unk0 != 0x162) {
        return -1;
    }
    if (*(u16 *)((u8 *)arg0 + 0xB2) != 0) {
        return -1;
    }
    if (*(s32 *)((u8 *)arg0 + 0x34) & 8) {
        return -1;
    }
    if (!(*(s32 *)((u8 *)arg0 + 0x34) & 0x10)) {
        *(s32 *)((u8 *)arg0 + 0x34) |= 8;
        return -1;
    }
    temp_v1 = *(s16 *)((u8 *)arg0 + 0x16);
    var_s0 = 0;
    if (temp_v1 != 0) {
        if (temp_v1 >= 0xC9) {
            var_s0 = 0x8D;
            if (*(s32 *)((u8 *)arg0 + 0x34) & 0x40) {
                var_s0 = 0xE3;
            }
        } else if (temp_v1 >= 0x65) {
            var_s0 = 0x8C;
        } else if (temp_v1 >= 2) {
            var_s0 = 0x8B;
        }
    }
    if (*(s16 *)((u8 *)arg0 + 0xB6) == -1) {
        if (var_s0 != 0) {
            *(s32 *)((u8 *)arg0 + 0x34) |= 8;
            Ovl11SetHalfwordTriplet(sp10, var_s0, 0, 1);
            Ovl11InstallPrimaryObjectRequest((u16 *)sp10);
            Ovl11StartRecordEffectAboveThreshold((Ovl11DF72CResetView *)arg0);
        }
    } else if (var_s0 != 0) {
        state_base = (u8 *)D_8006C838;
        temp_v0_2 = (Ovl11DF84CStateView *)
            (state_base + (*(s16 *)((u8 *)arg0 + 0xB6) * 0xF8));
        if (temp_v0_2->pending == 0) {
            *(s32 *)((u8 *)arg0 + 0x34) |= 8;
            temp_v0_2->pending = (u16)var_s0;
            Ovl11StartRecordEffectAboveThreshold((Ovl11DF72CResetView *)arg0);
        } else {
            var_s0 = 0;
        }
    }
    if (var_s0 != 0) {
        far_base = (u8 *)&D_8007AFF0;
        if (*(s16 *)(far_base + 0x25476) == *(s16 *)((u8 *)arg0 + 0x30)) {
            ovl_11_func_800D12A0(9);
        }
    }
    var_v0 = 0;
    if (var_s0 == 0) {
        var_v0 = -1;
    }
    return var_v0;
}
