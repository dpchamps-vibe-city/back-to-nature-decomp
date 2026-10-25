#include "common.h"

/* View of the D_8007AFF0 + 0x25394 work area (fields at 0x25394/0x25398/0x253B6).
 * The struct-field address web (rather than a char* + folded constant offset)
 * is what reproduces the original's far-buffer load ordering. */
typedef struct {
    s32 f0;
    s32 f4;
    u8 pad[0x1A];
    u16 f22;
} FarWork;

void Ovl11ResetSceneCameraOrbit(void);
void func_8001F2EC(s32 cur, s32 span, s32 to, s32 from, s32 *out);

s32 ovl_11_func_800EE604(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 *temp_s0;
    s32 *base;
    s32 *temp_s2;
    s32 state;
    s32 count;
    s32 var_s3;

    var_s3 = 0;
    if (arg3 != 0) {
        Ovl11ResetSceneCameraOrbit();
    }
    base = D_80129560;
    temp_s2 = (s32 *)((u8 *)base + (arg0 * 4));
    state = *temp_s2;
    if (state != 1) {
        if (state < 2) {
            if (state == 0) {
                char *far_base = (char *)&D_8007AFF0;
                D_8012954C = -0x320;
                D_801295C6 = -1;
                *temp_s2 = 1;
                D_80129540 = ((FarWork *)(far_base + 0x25394))->f4;
                D_80129544 = ((FarWork *)(far_base + 0x25394))->f0;
                D_80129548 = ((FarWork *)(far_base + 0x25394))->f22;
                D_80129550 = ((FarWork *)(far_base + 0x25394))->f0 - 0x44C;
                D_80129554 = ((FarWork *)(far_base + 0x25394))->f22 - 0x3E8;
                goto done;
            }
        }
    } else {
        temp_s0 = &D_80129560[arg1];
        func_8001F2EC(*temp_s0, 0xB1, D_80129540, D_8012954C, &sp18);
        func_8001F2EC(*temp_s0, 0xB1, D_80129544, D_80129550, &sp1C);
        func_8001F2EC(*temp_s0, 0xB1, D_80129548, D_80129554, &sp20);
        {
            char *far_base = (char *)&D_8007AFF0;
            ((FarWork *)(far_base + 0x25394))->f4 = sp18;
            ((FarWork *)(far_base + 0x25394))->f0 = sp1C;
            ((FarWork *)(far_base + 0x25394))->f22 = sp20;
        }
        count = *temp_s0 + 1;
        *temp_s0 = count;
        if (count >= 0xB1) {
            *temp_s2 = 2;
        }
        goto done;
    }
    var_s3 = 1;
done:
    return var_s3;
}
