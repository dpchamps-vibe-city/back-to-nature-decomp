#include "common.h"
#include "game_flags.h"
void ovl_11_func_800F4CEC (void);
void ovl_11_func_800F4CAC (void);
void ovl_11_func_800CD0C0 (s32 *arg0);
void ovl_11_func_800D0FB4();

void ovl_11_func_80111F88(void) {
    s32 var_s0;
    Ovl11ProgressState *temp_v0_3;
    u32 temp_s1;

    if (((Ovl11ProgressState *)D_8006C838)->activeTimer == 0 ||
        --((Ovl11ProgressState *)D_8006C838)->activeTimer < 0) {
        SetGameFlag(0x37U, 0U);
        ((Ovl11ProgressState *)D_8006C838)->activeTimer = 0;
    }
    if (GetGameFlag(0x38U) == 1) {
        if (--((Ovl11ProgressState *)D_8006C838)->pendingTimer < 0) {
            ovl_11_func_800D0FB4();
            ovl_11_func_800F4CEC();
            ovl_11_func_800F4CAC();
            SetGameFlag(0x38U, 0U);
            ((Ovl11ProgressState *)D_8006C838)->pendingTimer = 0U;
            for (var_s0 = 0x3A; var_s0 < 0x3E; var_s0++) {
                if (GetGameFlag((u16)var_s0) == 0) {
                    break;
                }
            }
            temp_s1 = (u16)var_s0;
            switch (var_s0) {
            case 0x3A:
                ((Ovl11ProgressState *)D_8006C838)->stage2 = 1;
                break;
            case 0x3B:
                ((Ovl11ProgressState *)D_8006C838)->stage0 = 1;
                break;
            case 0x3C:
                ((Ovl11ProgressState *)D_8006C838)->stage1 = 1;
                break;
            case 0x3D:
                ((Ovl11ProgressState *)D_8006C838)->stage0 = 2;
                break;
            case 0x3E:
                temp_v0_3 = (Ovl11ProgressState *)D_8006C838;
                temp_v0_3->complete = 1;
                temp_v0_3->flags = (s32) (temp_v0_3->flags | 0x2000);
                SetGameFlag(0x39U, 1U);
                break;
            }
            SetGameFlag((u16) temp_s1, 1U);
            if (((Ovl11ProgressState *)D_8006C838)->mode == 2) {
                ovl_11_func_800CD0C0(&D_80075A1C);
            }
        }
    }
    if (((Ovl11ProgressState *)D_8006C838)->cooldown != 0) {
        if (--((Ovl11ProgressState *)D_8006C838)->cooldown < 0) {
            ((Ovl11ProgressState *)D_8006C838)->cooldown = 0;
        }
    }
}
