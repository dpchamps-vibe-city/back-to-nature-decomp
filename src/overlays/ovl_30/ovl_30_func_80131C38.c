#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"


void func_8001AF70(u16 arg0, u16 arg1);
s32 func_8001AF44(u32 arg0);

s32 ovl_30_func_80131C38(s32 arg0) {
    u32 var_s0;

    if ((*(s32 *) ((u8 *) gPad0Input + 0)) & 0x20) {
        D_801349AC += 5;
        if (D_801349AC >= 0x4A0) {
            D_801349AC = 0;
        }
    }
    if ((*(s32 *) ((u8 *) gPad0Input + 0)) & 0x40) {
        D_801349AC -= 5;
        if (D_801349AC <= 0) {
            D_801349AC = 0x49F;
        }
    }
    if ((*(s32 *) ((u8 *) gPad0Input + 0)) & 0x10) {
        D_801349AC += 0x32;
        if (D_801349AC >= 0x4A0) {
            D_801349AC = 0;
        }
    }
    if ((*(s32 *) ((u8 *) gPad0Input + 0)) & 0x80) {
        D_801349AC -= 0x32;
        if (D_801349AC <= 0) {
            D_801349AC = 0x49F;
        }
    }
    if ((*(s32 *) ((u8 *) gPad0Input + 0)) & 0x4000) {
        D_801349AC++;
        if (D_801349AC >= 0x4A0) {
            D_801349AC = 0;
        }
    }
    if ((*(s32 *) ((u8 *) gPad0Input + 0)) & 0x1000) {
        if (D_801349AC <= 0) {
            D_801349AC = 0x49F;
        } else {
            D_801349AC--;
        }
    }
    if ((*(s32 *) ((u8 *) gPad0Input + 8)) & 0x8000) {
        func_8001AF70((u16) (*(s32 *) ((u8 *) &D_801349AC + 0)), 1U);
    }
    if ((*(s32 *) ((u8 *) gPad0Input + 8)) & 0x2000) {
        func_8001AF70((u16) (*(s32 *) ((u8 *) &D_801349AC + 0)), 0U);
    }
    if (func_8001AF44((u32) (u16) (*(s32 *) ((u8 *) &D_801349AC + 0))) == 1) {
        FntPrint(&D_8012EC90, D_801349AC);
    } else {
        FntPrint(&D_8012ECA0, D_801349AC);
    }
    var_s0 = 0;
    do {
        if (func_8001AF44((u16)((u16)D_801349AC + var_s0 - ((D_801349AC + var_s0) / 0x4A0) * 0x4A0)) == 1) {
            FntPrint(D_8012ECB0, D_801349AC + var_s0, D_801349AC + var_s0 - 0xFB);
        } else {
            FntPrint(D_8012ECC4, D_801349AC + var_s0, D_801349AC + var_s0 - 0xFB);
        }
        var_s0++;
    } while (var_s0 < 0xAU);
    return arg0;
}
/* Warning: struct GfxObj is not defined (only forward-declared) */
