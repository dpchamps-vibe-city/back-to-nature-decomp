#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "cd_audio.h"
#include "master_volume.h"

void ovl_30_func_8012F44C(void);
void ovl_30_func_8012F410(void);
void ovl_30_func_8012F3D4(void);
s32 ovl_30_func_8012FE68(s32, s32);
void ovl_30_func_8012FDEC(void);
void func_8001FAE8(s32);
s32 func_8001FBBC(s16);
s32 func_80020818(void);
s32 func_80020B80(s32, s32);
void func_8001FBF0(s16, s16);
s32 func_8001FABC(s16);
s32 func_8001FB30(s16, s16, s16, s16);

#define DEBUG_FLAGS ((struct Ovl30DebugFlags *)D_8006C838)
#define INPUT ((SomeStruct *)gPad0Input)

s32 ovl_30_func_8012F454(s32 state) {
    s32 flags[15];
    u32 i;

    ovl_30_func_8012F44C();
    for (i = 0; i < 15; i++) {
        flags[i] = 0;
    }
    if (DEBUG_FLAGS->flags10 < 0) flags[0] = 1;
    if (DEBUG_FLAGS->flags10 & 0x40000000) flags[1] = 1;
    if (!(DEBUG_FLAGS->flags10 & 0x20000000)) flags[2] = 1;
    if (DEBUG_FLAGS->flags10 & 0x10000000) flags[3] = 1;
    if (DEBUG_FLAGS->flags10 & 0x08000000) flags[4] = 1;
    if (DEBUG_FLAGS->flags10 & 0x04000000) flags[10] = 1;
    if (DEBUG_FLAGS->flagsC & 0x00800000) flags[11] = 1;
    if (DEBUG_FLAGS->flags10 & 0x02000000) flags[12] = 1;
    if (DEBUG_FLAGS->flags10 & 0x01000000) flags[13] = 1;
    if (DEBUG_FLAGS->flags10 & 0x00800000) flags[14] = 1;

    switch (state) {
    case 0:
        state = 1;
        D_80134AE4 = 0;
        D_80134AE8 = 0;
        D_80134AEC = 0;
        D_80134AF0 = 0;
        D_80134AF4 = 0;
        D_80134AF8 = -1;
        D_80134B14 = ((struct Ovl30SettingsView *)&D_8007AFF0)->selected;
        break;
    case 1:
        FntPrint(D_8012DEC4);
        FntPrint(D_8012DEE0, D_80134B14);
        FntPrint(D_8012DEF0, D_80134AE8);
        FntPrint(D_8012DF00, D_80134AEC);
        FntPrint(D_8012DF14, D_80134AF0);
        FntPrint(D_8012DF20, D_80134AF4);
        if (INPUT->field_0x0 & 0x1000) {
            if (D_80134AE4 == 8) {
                state = 1000;
                ovl_30_func_8012F410();
            }
            if (D_80134AE4 == 0) {
                D_80134AE4 = D_80133FB8 - 1;
            } else {
                D_80134AE4--;
            }
        } else if (INPUT->field_0x0 & 0x4000) {
            if (D_80134AE4 == 8) {
                state = 1000;
                ovl_30_func_8012F410();
            }
            if (D_80134AE4 == D_80133FB8 - 1) {
                D_80134AE4 = 0;
            } else {
                D_80134AE4++;
            }
        } else {
            goto display_rows;
        }
        if (D_80134AE4 == 8) {
            state = 1000;
            ovl_30_func_8012F3D4();
        }
        break;
    display_rows:
        for (i = 0; i < D_80133FB8; i++) {
            if (i == D_80134AE4) {
                FntPrint(D_8012DF2C);
            } else {
                FntPrint(D_8012DF30);
            }
            FntPrint(D_8012DF34, D_80133FB4[i], D_80133F70[flags[i]]);
        }
        switch (D_80134AE4) {
        case 10:
            if (INPUT->field_0x8 & 0x2000) ovl_30_func_8012FE68(26, 1);
            if (INPUT->field_0x8 & 0x8000) ovl_30_func_8012FE68(26, 0);
            break;
        case 11:
            if (INPUT->field_0x8 & 0x2000) DEBUG_FLAGS->flagsC |= 0x800000;
            if (INPUT->field_0x8 & 0x8000) DEBUG_FLAGS->flagsC &= ~0x800000;
            break;
        case 12:
            if (INPUT->field_0x8 & 0x2000) DEBUG_FLAGS->flags10 |= 0x02000000;
            if (INPUT->field_0x8 & 0x8000) DEBUG_FLAGS->flags10 &= ~0x02000000;
            break;
        case 13:
            if (INPUT->field_0x8 & 0x2000) DEBUG_FLAGS->flags10 |= 0x01000000;
            if (INPUT->field_0x8 & 0x8000) DEBUG_FLAGS->flags10 &= ~0x01000000;
            break;
        case 14:
            if (INPUT->field_0x8 & 0x2000) DEBUG_FLAGS->flags10 |= 0x800000;
            if (INPUT->field_0x8 & 0x8000) DEBUG_FLAGS->flags10 &= ~0x800000;
            break;
        default:
            if (INPUT->field_0x8 & 0x2000) ovl_30_func_8012FE68(31 - D_80134AE4, 1);
            if (INPUT->field_0x8 & 0x8000) ovl_30_func_8012FE68(31 - D_80134AE4, 0);
            break;
        case 5:
            ovl_30_func_8012FDEC();
            break;
        case 6:
            if (INPUT->field_0x8 & 0x20) {
                if (D_80134AF8 != -1) func_8001FAE8(D_80134AF8);
                D_80134AF8 = func_8001FABC(D_80134AE8);
            } else {
                if (INPUT->field_0x0 & 0x2000) {
                    if (++D_80134AE8 >= 77) D_80134AE8 = 0;
                }
                if (INPUT->field_0x0 & 0x8000) {
                    if (--D_80134AE8 < 0) D_80134AE8 = 76;
                }
            }
            break;
        case 7:
            if (INPUT->field_0x8 & 0x20) {
                state = 999;
                func_8001FBBC(0);
                func_80020818();
                func_80020B80(2, 0);
                func_8001FBF0(D_80134AF0 + 1, 0);
            } else {
                if (INPUT->field_0x0 & 0x2000) {
                    if (++D_80134AF0 >= 37) D_80134AF0 = 0;
                }
                if (INPUT->field_0x0 & 0x8000) {
                    if (--D_80134AF0 < 0) D_80134AF0 = 35;
                }
            }
            break;
        case 8:
            if (INPUT->field_0x8 & 0x20) {
                if (D_80134AF8 != -1) func_8001FAE8(D_80134AF8);
                D_80134AF8 = func_8001FABC(D_80134AEC + 54);
            } else {
                if (INPUT->field_0x0 & 0x2000) {
                    if (++D_80134AEC >= 16) D_80134AEC = 0;
                }
                if (INPUT->field_0x0 & 0x8000) {
                    if (--D_80134AEC < 0) D_80134AEC = 15;
                }
            }
            break;
        case 9:
            if (INPUT->field_0x8 & 0x20) {
                StartCdAudioTrack(D_80134AF4);
            } else {
                if (INPUT->field_0x0 & 0x2000) {
                    if (++D_80134AF4 >= 26) D_80134AF4 = 0;
                }
                if (INPUT->field_0x0 & 0x8000) {
                    if (--D_80134AF4 < 0) D_80134AF4 = 25;
                }
            }
            break;
        }
        break;
    case 999:
        if (IsSoundTransferComplete()) {
            state = 1;
            func_80020818();
            func_8001FB30(0, 0, D_80134AF0, 127);
        }
        break;
    case 1000:
        if (IsSoundTransferComplete()) {
            state = 1;
            func_80020818();
        }
        break;
    }
    return state;
}
