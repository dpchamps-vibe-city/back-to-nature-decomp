#include "common.h"
#include "game_flags.h"

s32 ovl_11_func_800EFA1C(s16);
s32 ovl_11_func_800F3820(void);
s32 ovl_11_func_800F3C9C(u16);
s32 ovl_11_func_800FEB68(void);
s32 ovl_11_func_800FEBB0(void);
void ovl_11_func_800F2538(s16, s16, s16);

void ovl_11_func_800F291C(void) {
    u8 *base;
    Ovl11EventFlagView *events;
    s16 *var_a0;
    s16 temp_s3;
    s16 temp_s6;
    s16 var_s1;
    s32 temp_v0;
    s32 var_v1_2;
    Ovl11EventStateView *var_v1;
    void *temp_s0;
    void *temp_s0_2;
    void *temp_s0_3;
    Ovl11EventStateView *temp_v1;

    events = (Ovl11EventFlagView *)((u8 *)D_8006C838 + 0x44FC);
    temp_v1 = (Ovl11EventStateView *) ((u8 *) events - 0x44FC);
    temp_s3 = temp_v1->field_44B8.halves.high;
    temp_s6 = temp_v1->field_44BC;
    if ((~temp_s3 == -1) && (temp_s6 == 0)) {
        var_s1 = temp_s3;
        if (GetGameFlag(0xE6U) == 1) {
            var_s1 = events->field_690A >= 0x64;
            goto block_6;
        }
        if (events->field_690A >= 0xC8) {
            var_s1 = 2;
        block_6:
            if (((((Ovl11EventStateView *)D_8006C838)->field_AE06) >= 0xC8) && (GetGameFlag(0x22U) == 1)) {
                var_s1 = 3;
            }
        }
        if (var_s1 != 0) {
            ovl_11_func_800F2538(2, var_s1, 1);
        }
        var_s1 = ((((Ovl11EventStateView *)D_8006C838)->field_A88A) >= 0x64) * 4;
        if (((((Ovl11EventStateView *)D_8006C838)->field_A88A) >= 0xC8) && (GetGameFlag(0x23U) == 1)) {
            var_s1 = 5;
        }
        if (var_s1 != 0) {
            ovl_11_func_800F2538(2, var_s1, 0);
        }
        var_s1 = 0;
        if ((((Ovl11EventStateView *)D_8006C838)->field_BCA6) >= 0x64) {
            var_s1 = 6;
            if (GetGameFlag(0x25U) == 1) {
                var_s1 = 7;
            }
        }
        if (GetGameFlag(0x20U) == 1) {
            var_s1 = 8;
        }
        if (((((Ovl11EventStateView *)D_8006C838)->field_BCA6) >= 0xC8) && (GetGameFlag(0x21U) == 1)) {
            var_s1 = 9;
        }
        if (var_s1 != 0) {
            ovl_11_func_800F2538(2, var_s1, 2);
        }
        var_s1 = 0;
        if ((((Ovl11EventStateView *)D_8006C838)->field_A30E) >= 0x64) {
            var_s1 = 0xA;
        }
        if (((((Ovl11EventStateView *)D_8006C838)->field_A30E) >= 0xC8) && (GetGameFlag(0x24U) == 1)) {
            var_s1 = 0xB;
        }
        if (var_s1 != 0) {
            ovl_11_func_800F2538(2, var_s1, 3);
        }
        do {
            var_s1 = 0;
            if ((((Ovl11EventStateView *)D_8006C838)->field_C79E) >= 0x64) {
                var_s1 = 0xC;
            }
            if ((((Ovl11EventStateView *)D_8006C838)->field_C79E) >= 0xC8) {
                temp_v0 = GetGameFlag(0x25U);
                if (temp_v0 == 1) {
                    if (ovl_11_func_800EFA1C(0) == temp_v0) {
                        var_s1 = 0xE;
                    } else if (GetGameFlag(0x11EU) == temp_v0) {
                        var_s1 = 0xD;
                    }
                }
            }
            if (var_s1 != 0) {
                ovl_11_func_800F2538(2, var_s1, 4);
            }
        } while (0);
        ovl_11_func_800F2538(2, 0xF, 1);
        base = (u8 *)D_8006C838;
        if ((((Ovl11EventStateView *)D_8006C838)->field_A13A) >= 0x96) {
            ovl_11_func_800F2538(2, 0x10, 0);
        }
        var_s1 = 0x11;
        if (GetGameFlag(0x1DU) == 1) {
            var_s1 = 0x12;
        }
        ovl_11_func_800F2538(2, var_s1, 2);
        ovl_11_func_800F2538(2, 0x13, 4);
        if ((((Ovl11EventStateView *)D_8006C838)->field_AFDA) >= 0x96) {
            ovl_11_func_800F2538(2, 0x14, 1);
        }
        if ((((Ovl11EventStateView *)D_8006C838)->field_B72A) >= 0x96) {
            ovl_11_func_800F2538(2, 0x15, 0);
        }
        var_s1 = 0x16;
        if ((((Ovl11EventStateView *)D_8006C838)->field_B8FE) >= 0x96) {
            var_s1 = 0x17;
        }
        ovl_11_func_800F2538(2, var_s1, 0);
        if ((((Ovl11EventStateView *)D_8006C838)->field_C04E) >= 0x96) {
            ovl_11_func_800F2538(2, 0x18, 1);
        }
        if ((((Ovl11EventStateView *)D_8006C838)->field_C3F6) >= 0x64) {
            ovl_11_func_800F2538(2, 0x19, 3);
        }
        var_s1 = 0x1A;
        var_a0 = (s16 *) (base + 0xDE16);
        for (var_v1_2 = 0; var_v1_2 < 7; var_v1_2++) {
            if (*var_a0 < 0x64) {
                var_s1 = 0;
            }
            var_a0 += 0x7C;
        }
        if (var_s1 != 0) {
            ovl_11_func_800F2538(2, var_s1, 0);
        }
        var_s1 = 0;
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_BE7C) > 0xEA5FU) && (GetGameFlag(0x25U) == 0) && (ovl_11_func_800F3820() == 1)) {
            var_s1 = 0x1B;
        }
        if (var_s1 != 0) {
            ovl_11_func_800F2538(2, var_s1, 4);
        }
        var_s1 = 0;
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_A6B8) > 0xEA5FU) && (GetGameFlag(0x22U) == 0) && (ovl_11_func_800F3820() == 1)) {
            var_s1 = 0x1C;
        }
        if (var_s1 != 0) {
            ovl_11_func_800F2538(2, var_s1, 3);
        }
        var_s1 = 0;
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_9F68) > 0xEA5FU) && (GetGameFlag(0x21U) == 0) && (ovl_11_func_800F3820() == 1)) {
            var_s1 = 0x1D;
        }
        if (var_s1 != 0) {
            ovl_11_func_800F2538(2, var_s1, 2);
        }
        var_s1 = 0;
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_AA60) > 0xEA5FU) && (GetGameFlag(0x23U) == 0) && (ovl_11_func_800F3820() == 1)) {
            var_s1 = 0x1E;
        }
        if (var_s1 != 0) {
            ovl_11_func_800F2538(2, var_s1, 0);
        }
        var_s1 = 0;
        if ((GetGameFlag(0x24U) == 0) && (ovl_11_func_800F3820() == 1)) {
            var_s1 = 0x1F;
            if ((u16) (((Ovl11EventStateView *)D_8006C838)->field_B384) > 0xEA5FU) {
                var_s1 = 0x20;
            }
        }
        if (var_s1 != 0) {
            ovl_11_func_800F2538(2, var_s1, 1);
        }
    }
    var_v1 = (Ovl11EventStateView *)D_8006C838;
    if ((temp_s3 == (var_v1->field_5246)) && (temp_s6 == (var_v1->field_5248))) {
        if (((u16) (var_v1->field_BE7C) > 0x9C3FU) && (GetGameFlag(0x20U) == 0)) {
            ovl_11_func_800F2538(2, 0x29, 0);
        }
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_A6B8) > 0x9C3FU) && (GetGameFlag(0x1DU) == 0)) {
            ovl_11_func_800F2538(2, 0x2A, 0);
        }
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_9F68) > 0x9C3FU) && (GetGameFlag(0x1CU) == 0)) {
            ovl_11_func_800F2538(2, 0x2B, 0);
        }
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_AA60) > 0x9C3FU) && (GetGameFlag(0x1EU) == 0)) {
            ovl_11_func_800F2538(2, 0x2C, 0);
        }
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_B384) > 0x9C3FU) && (GetGameFlag(0x1FU) == 0)) {
            ovl_11_func_800F2538(2, 0x2D, 0);
        }
        if (temp_s3 != 1) {
            if ((((Ovl11EventStateView *)D_8006C838)->field_C79E) >= 0xC8) {
                ovl_11_func_800F2538(2, 0x2E, 0);
            }
        }
        if (((((Ovl11EventStateView *)D_8006C838)->field_AE06) >= 0xC8) && (GetGameFlag(0x129U) == 1)) {
            ovl_11_func_800F2538(2, 0x2F, 0);
        }
        if ((((Ovl11EventStateView *)D_8006C838)->field_A88A) >= 0xC8) {
            ovl_11_func_800F2538(2, 0x30, 0);
        }
    }
    if (!(events->flags & 1) && ((temp_s3 == 0) || ((((Ovl11EventStateView *)D_8006C838)->field_44D4) == 1))) {
        temp_s0 = (void *)D_8006C838;
        if ((((Ovl11EventStateView *)temp_s0)->field_498C[ovl_11_func_800F3C9C(0x41U)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0)->field_498C[ovl_11_func_800F3C9C(0x42U)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0)->field_498C[ovl_11_func_800F3C9C(0x43U)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0)->field_498C[ovl_11_func_800F3C9C(0x45U)] >= 0x64U)) {
            ovl_11_func_800F2538(2, 0x38, 0);
            events->flags = (s32) (events->flags | 1);
        }
    }
    if (!(events->flags & 2) && ((temp_s3 == 1) || ((((Ovl11EventStateView *)D_8006C838)->field_44D4) == 1))) {
        temp_s0_2 = (void *)D_8006C838;
        if ((((Ovl11EventStateView *)temp_s0_2)->field_498C[ovl_11_func_800F3C9C(0x46U)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0_2)->field_498C[ovl_11_func_800F3C9C(0x47U)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0_2)->field_498C[ovl_11_func_800F3C9C(0x48U)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0_2)->field_498C[ovl_11_func_800F3C9C(0x4AU)] >= 0x64U)) {
            ovl_11_func_800F2538(2, 0x39, 0);
            events->flags = (s32) (events->flags | 2);
        }
    }
    if (!(events->flags & 4) && ((temp_s3 == 2) || ((((Ovl11EventStateView *)D_8006C838)->field_44D4) == 1))) {
        temp_s0_3 = (void *)D_8006C838;
        if ((((Ovl11EventStateView *)temp_s0_3)->field_498C[ovl_11_func_800F3C9C(0x4BU)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0_3)->field_498C[ovl_11_func_800F3C9C(0x4CU)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0_3)->field_498C[ovl_11_func_800F3C9C(0x4DU)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0_3)->field_498C[ovl_11_func_800F3C9C(0x4FU)] >= 0x64U)) {
            ovl_11_func_800F2538(2, 0x3A, 0);
            events->flags = (s32) (events->flags | 4);
        }
    }
    if (!(events->flags & 8) && ((temp_s3 == 3) || ((((Ovl11EventStateView *)D_8006C838)->field_44D4) == 1)) && (GetGameFlag(0x10EU) == 1)) {
        ovl_11_func_800F2538(2, 0x3B, 0);
        events->flags = (s32) (events->flags | 8);
    }
    if (!(events->flags & 0x10) && (~((((Ovl11EventStateView *)D_8006C838)->field_C) & 0x200000) != 0) && (((u16) (((Ovl11EventStateView *)D_8006C838)->field_9F68) > 0xC34FU) || ((u16) (((Ovl11EventStateView *)D_8006C838)->field_A6B8) > 0xC34FU) || ((u16) (((Ovl11EventStateView *)D_8006C838)->field_AA60) > 0xC34FU) || ((u16) (((Ovl11EventStateView *)D_8006C838)->field_B384) > 0xC34FU) || ((u16) (((Ovl11EventStateView *)D_8006C838)->field_BE7C) > 0xC34FU))) {
        ovl_11_func_800F2538(2, 0x3C, 0);
        events->flags = (s32) (events->flags | 0x10);
    }
    if (!(events->flags & 0x20) && ((((Ovl11EventStateView *)D_8006C838)->field_51C0) & 0x80)) {
        ovl_11_func_800F2538(2, 0x3D, 0);
        events->flags = (s32) (events->flags | 0x20);
    }
    if (!(events->flags & 0x40) && ((((Ovl11EventStateView *)D_8006C838)->field_51C0) & 1)) {
        ovl_11_func_800F2538(2, 0x3E, 0);
        events->flags = (s32) (events->flags | 0x40);
    }
    if (!(events->flags & 0x80) && ((((Ovl11EventStateView *)D_8006C838)->field_51C0) & 2)) {
        ovl_11_func_800F2538(2, 0x3F, 0);
        events->flags = (s32) (events->flags | 0x80);
    }
    if (!(events->flags & 0x100) && ((((Ovl11EventStateView *)D_8006C838)->field_51C0) & 0x40000) && ((ovl_11_func_800FEB68() > 0) || (ovl_11_func_800FEBB0() > 0) || ((((Ovl11EventStateView *)D_8006C838)->field_929C) != 0))) {
        ovl_11_func_800F2538(2, 0x40, 0);
        events->flags = (s32) (events->flags | 0x100);
    }
    if (!(events->flags & 0x200) && ((((Ovl11EventStateView *)D_8006C838)->field_51C0) & 0x80000) && (ovl_11_func_800FEB68() > 0)) {
        ovl_11_func_800F2538(2, 0x41, 0);
        events->flags = (s32) (events->flags | 0x200);
    }
    if (!(events->flags & 0x400) && ((((Ovl11EventStateView *)D_8006C838)->field_51C0) & 0x100000) && (ovl_11_func_800FEBB0() > 0)) {
        ovl_11_func_800F2538(2, 0x42, 0);
        events->flags = (s32) (events->flags | 0x400);
    }
    if (!(events->flags & 0x800) && (GetGameFlag(0x24U) == 1)) {
        ovl_11_func_800F2538(2, 0x43, 1);
        events->flags = (s32) (events->flags | 0x800);
    }
    if (!(events->flags & 0x1000) && ((((Ovl11EventStateView *)D_8006C838)->field_44B8.word) == 0x10000) && ((((Ovl11EventStateView *)D_8006C838)->field_44BC) == 1)) {
        ovl_11_func_800F2538(2, 0x26, 0);
        events->flags = (s32) (events->flags | 0x800);
    }
    if (((((Ovl11EventStateView *)D_8006C838)->field_44B8.halves.high) == 2) && ((((Ovl11EventStateView *)D_8006C838)->field_44BC) == 0)) {
        ovl_11_func_800F2538(2, 0x27, 0);
    }
    if (!(events->flags & 0x2000) && ((((Ovl11EventStateView *)D_8006C838)->field_44B8.halves.high) == 1) && ((((Ovl11EventStateView *)D_8006C838)->field_44BC) >= 0) && ((((Ovl11EventStateView *)D_8006C838)->field_44BC) < 0x11) && ((((Ovl11EventStateView *)D_8006C838)->field_5492) == -1) && (GetGameFlag(0x22U) == 0) && ((u16) (((Ovl11EventStateView *)D_8006C838)->field_A6B8) >= 0x3A99U)) {
        ovl_11_func_800F2538(2, 0x19B, 0);
        events->flags = (s32) (events->flags | 0x2000);
    }
}
