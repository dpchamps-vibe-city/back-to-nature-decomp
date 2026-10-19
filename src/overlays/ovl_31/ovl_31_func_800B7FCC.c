#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libapi.h"

s32 ovl_31_func_800B82E8(void);
s32 ovl_31_func_800B8348(void);
s32 ovl_31_func_800B83B8(void);
s32 ovl_31_func_800B8490(void);
s32 ovl_31_func_800B8600(void);

void ovl_31_func_800B7FCC(void) {
    s32 index;
    s32 result;
    u32 buttons;

    FntPrint(&D_800B7E24);
    for (index = 0; index < 6; index++) {
        FntPrint(D_800B7E40, D_800B87F0[index]);
    }
    FntPrint(D_800B7E48);
    if (D_800B886C == -1) {
        FntPrint(D_800B7E4C);
        if (((GfxObj *) D_8005E3A8)->field_8) D_800B886C = 0;
        return;
    }
    if (D_800B886C == 1) {
        FntPrint(D_800B7E64);
        if (((GfxObj *) D_8005E3A8)->field_8) D_800B886C = 0;
        return;
    }
    if (D_800B886C == 2) {
        for (index = 0; index < *(s32 *) D_800B8868; index++) {
            FntPrint(D_800B7E78, (&D_800B88B0)[index].name);
        }
        if ((*(s32 *) D_800B8868) != 0) {
            FntPrint(D_800B7E48);
        }
        FntPrint(D_800B7E78, D_800B8B10);
        if (((GfxObj *) D_8005E3A8)->field_8 != 0) {
            D_800B886C = 0;
        }
        return;
    } else if (D_800B886C != 0) {
        FntPrint(D_800B7E7C);
        switch (D_800B886C) {
        case -2:
            FntPrint(D_800B7E88);
            break;
        case -3:
            FntPrint(D_800B7E90);
            break;
        case -4:
            FntPrint(D_800B7EA0);
            break;
        case -5:
            FntPrint(D_800B7EA8);
            break;
        case -6:
            FntPrint(D_800B7EB0);
            break;
        case -7:
            FntPrint(D_800B7EC0);
            break;
        case -8:
            FntPrint(D_800B7EC8);
            break;
        }
        if ((((GfxObj *) D_8005E3A8)->field_8) != 0) {
            D_800B886C = 0;
        }
    } else {
        buttons = ((GfxObj *) D_8005E3A8)->field_8;
        result = 0;
        switch (buttons) {
        case 0x800:
            func_80011EF0(0);
            break;
        case 0x10:
            result = ovl_31_func_800B8348();
            break;
        case 0x80:
            result = ovl_31_func_800B82E8();
            break;
        case 0x40:
            result = ovl_31_func_800B83B8();
            break;
        case 0x20:
            result = ovl_31_func_800B8490();
            break;
        case 0x100:
            result = ovl_31_func_800B8600();
            break;
        }
        if (result != 0) {
            D_800B886C = result;
        }
    }
}
