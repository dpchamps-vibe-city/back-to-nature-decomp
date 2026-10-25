#include "common.h"
#include "master_volume.h"

void ovl_28_func_800B8A20(void);
void ovl_28_func_800B8AA8(void);
void ovl_28_func_800B8994(s16 arg0);

void ovl_28_func_800B8478(void) {
    s32 temp_a1;
    s32 temp_lo;
    u8 temp_v1;

    D_800B93B0 = (u16) D_800B93B0 + 1;
    ovl_28_func_800B8A20();
    if (D_800B93B8 == 1) {
        ovl_28_func_800B8AA8();
    }
    if (D_800B93B2 > 0) {
        if (D_800B93B0 >= D_800B93B2) {
            D_800B93B2 = 0;
            goto loop_5;
        }
    } else {
loop_5:
        temp_v1 = D_800B936C[D_800B93AE];
        temp_lo = 0x10000 / D_800B93B4;
        temp_a1 = temp_v1 & 0xF0;
        D_800B93B2 = (s16) temp_lo;
        D_800B93B0 = 0;
        switch (temp_a1) {
        case 0xF0:
            if (temp_v1 == 0xFF) {
                D_800B93BC = 1;
            } else if (temp_v1 == 0xF3) {
                D_800B93C0 = 0;
            } else if (temp_v1 == 0xF2) {
                D_800B93C0 = 1;
            } else if (temp_v1 == 0xF1) {
                D_800B93B8 = 0;
            } else if (temp_v1 == temp_a1) {
                BeginMasterVolumeFadeOut(0x3C);
            }
            D_800B93AE = (u16) D_800B93AE + 1;
            goto loop_5;
        case 0xE0:
            D_800B93B2 = temp_lo + (temp_v1 & 0xF) * 0x1E;
            break;
        case 0xD0:
            D_800B93B2 = temp_lo + (0xE000 / (s32) D_800B93B4) * (temp_v1 & 0xF);
            break;
        default:
            ovl_28_func_800B8994((s16) temp_v1);
            break;
        }
        D_800B93AE = (u16) D_800B93AE + 1;
    }
}
