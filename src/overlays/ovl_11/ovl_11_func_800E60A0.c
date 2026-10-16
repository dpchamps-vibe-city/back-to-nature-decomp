#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/abs.h"

/* Legacy callers supply four arguments; the callee consumes only two. */
s32 ovl_11_func_800E63C8();

s32 ovl_11_func_800E60A0(s16 arg0, s32 height, s32 arg2, s32 arg3) {
    s16 arg1 = height;
    VECTOR delta;
    s16 temp_s0;
    s32 temp_lo;
    s16 var_s1;
    s32 var_t0;
    s32 var_v0;
    s32 var_v1_2;
    s32 *position;

    temp_s0 = arg0;
    D_801295C6 = -1;
    ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x400000;
    ovl_11_func_800E63C8(0, 0, 0, 0);
    var_s1 = 0x18;
    temp_s0 += 2;
    switch (temp_s0) {
    case 1: {
        u8 *far_base = (u8 *)&D_8007AFF0;
        *(s16 *)(far_base + 0x253B6) = arg1;
        *(s16 *)(far_base + 0x253B4) = arg2;
        *(s16 *)(far_base + 0x253B8) = arg3;
        return 1;
    }
    case 0:
        var_s1 = 5;
        break;
    case 2:
        var_s1 = 0x18;
        break;
    case 3:
        var_s1 = 0x24;
        break;
    case 4:
        var_s1 = 0x30;
        break;
    case 5:
        var_s1 = 0x48;
        break;
    }
    if (D_80124908 == 0) {
        u8 *far_base = (u8 *)&D_8007AFF0;
        D_80124908 = 1;
        D_801294F0[0] = *(s16 *)(far_base + 0x253B4);
        D_801294F0[1] = *(s16 *)(far_base + 0x253B6);
        D_801294F0[2] = *(s16 *)(far_base + 0x253B8);
    }
    position = D_801294F0;
    delta.vy = abs(arg1 - position[1]);
    delta.vx = abs(arg2 - D_801294F0[0]);
    delta.vz = abs(arg3 - position[2]);
    var_v0 = SquareRoot0(delta.vx * delta.vx + delta.vz * delta.vz);
    var_v1_2 = var_v0 < delta.vy ? delta.vy : var_v0;
    temp_lo = var_v1_2 / var_s1;
    if (temp_lo != 0) {
        goto interpolate;
    }
    {
        var_t0 = 0;
        if (position[2] == arg3 && D_801294F0[0] == arg2 &&
            position[1] == arg1) {
            D_80124908 = 0;
            var_t0 = 1;
        } else {
            u8 *far_base = (u8 *)&D_8007AFF0;
            var_t0 = 0;
            D_801294F0[2] = arg3;
            D_801294F0[0] = arg2;
            D_801294F0[1] = arg1;
            *(s16 *)(far_base + 0x253B8) = (s16)(u16)D_801294F0[2];
            *(s16 *)(far_base + 0x253B4) = (s16)(u16)D_801294F0[0];
            *(s16 *)(far_base + 0x253B6) = (s16)(u16)D_801294F0[1];
        }
    }
    goto done;
interpolate:
    {
        D_801294F0[2] += (arg3 - D_801294F0[2]) / temp_lo;
        D_801294F0[0] += (arg2 - D_801294F0[0]) / temp_lo;
        D_801294F0[1] += (arg1 - D_801294F0[1]) / temp_lo;
        var_t0 = 0;
        {
            u8 *far_base = (u8 *)&D_8007AFF0;
            *(s16 *)(far_base + 0x253B8) = (s16)(u16)D_801294F0[2];
            *(s16 *)(far_base + 0x253B4) = (s16)(u16)D_801294F0[0];
            *(s16 *)(far_base + 0x253B6) = (s16)(u16)D_801294F0[1];
        }
    }
done:
    return var_t0;
}
