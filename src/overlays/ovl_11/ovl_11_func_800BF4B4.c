#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 ovl_11_func_8011E090(VECTOR *pos, u16 *colOut, u16 *rowOut);
s32 ovl_11_func_800D7EF8(s32 *pos, u16 *colOut, u16 *rowOut);
s32 ovl_11_func_800BF630(s16 col, s16 row, s32 map);

s32 ovl_11_func_800BF4B4(s32 *arg0, s32 arg1) {
    VECTOR nearPos;
    VECTOR farPos;
    u16 nearCol;
    u16 nearRow;
    u16 farCol;
    u16 farRow;
    s16 *delta;
    s32 secondResult;
    s32 map;
    s32 result;

    map = (((struct Ovl11D2764StateView *) &D_8007AFF0)->field_25476 != 1) * 2;
    if (((struct Ovl11D2764StateView *) &D_8007AFF0)->field_25476 == 6) {
        map = 1;
    }
    if (map != 2) {
        delta = (s16 *) (gOvl11AreaGridCellOffsets + arg1 * 4);
        nearPos.vx = arg0[0] + delta[0];
        nearPos.vy = arg0[1];
        nearPos.vz = arg0[2] + delta[1];
        farPos.vx = arg0[0] + delta[0] * 2;
        farPos.vy = arg0[1];
        farPos.vz = arg0[2] + delta[1] * 2;
        if (map != 0) {
            result = 0;
            if (ovl_11_func_8011E090(&nearPos, &nearCol, &nearRow) == 0) {
                secondResult = ovl_11_func_8011E090(&farPos, &farCol, &farRow);
                goto check;
            }
            return result;
        }
        result = 0;
        if ((ovl_11_func_800D7EF8(&nearPos.vx, &nearCol, &nearRow) == 0) &&
            (((s16) nearCol != 0x2C) || (result = 0, arg1 != 3))) {
            secondResult = ovl_11_func_800D7EF8(&farPos.vx, &farCol, &farRow);
check:
            if (ovl_11_func_800BF630((s16) nearCol, (s16) nearRow, map) == 1) {
                if (secondResult == 0) {
                    if (ovl_11_func_800BF630((s16) farCol, (s16) farRow, map) == 2) {
                        return 1;
                    }
                } else {
                    return 1;
                }
            }
            return 0;
        }
        return result;
    }
    return 0;
}
