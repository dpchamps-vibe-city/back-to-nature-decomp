#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void func_8001AF70 (u16 arg0, u16 arg1);
s32 ovl_11_func_800EF0E8 (s16 arg0);
void ovl_11_func_800EF488 (s16 arg0);
s32 func_8001AF44 (u32 arg0);
s32 ovl_11_func_800EF8BC (s16 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800EFA1C (s16 arg0);
void ovl_11_func_80112904 (s16 arg0);

void ovl_11_func_800EEECC(void) {
    s16 var_s0;

    func_8001AF70(0x26U, 0U);
    func_8001AF70(0x27U, 0U);
    func_8001AF70(0x28U, 0U);
    func_8001AF70(0x29U, 0U);
    func_8001AF70(0x2AU, 0U);
    for (var_s0 = 0; var_s0 < 5; var_s0++) {
        if (ovl_11_func_800EF0E8(var_s0) == 1) {
            ovl_11_func_800EF488(var_s0);
        }
    }
    if (((struct Ovl11ButtonUpdateState *) D_8006C838)->field_44BA == 0 &&
        ((struct Ovl11ButtonUpdateState *) D_8006C838)->field_44BC == 7 &&
        func_8001AF44(0x1BU) == 0) {
        ((struct Ovl11ButtonUpdateState *) D_8006C838)->field_E668 = 0;
    }
}
