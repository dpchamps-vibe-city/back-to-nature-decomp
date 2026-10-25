#include "common.h"
#include "game_types.h"
#include "sound_commands.h"

void ovl_11_func_800FE704(void);
void ovl_11_func_800FE780(void);

void ovl_11_func_800FD034(s16 arg0, void (*arg1)(s16, s16)) {
    void (*draw)(s16, s16);
    s32 i;
    s16 y;

    draw = arg1;
    if (arg0 >= 8) {
        if (((SomeStruct *) gPad0Input)->field_0x0 & 0x1000) {
            PlaySoundEffect(5);
            D_80127210--;
        } else if (((SomeStruct *) gPad0Input)->field_0x0 & 0x4000) {
            PlaySoundEffect(5);
            D_80127210++;
        }
    }
    if (arg0 < D_80127210 + 7) {
        D_80127210 = 0;
    }
    if (D_80127210 < 0) {
        D_80127210 = arg0 - 7;
    }
    if (D_80127210 > 0) {
        ovl_11_func_800FE704();
    }
    if (D_80127210 + 7 < arg0) {
        ovl_11_func_800FE780();
    }
    i = D_80127210;
    y = 0x30;
    while (i < D_80127210 + 7) {
        i += 1;
        i <<= 16;
        i >>= 16;
        draw(i, y);
        y += 0x18;
    }
}
