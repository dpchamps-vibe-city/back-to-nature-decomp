#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void func_80015840 (ObjectState *obj, s8 arg1);
void ovl_25_func_800BA5BC (SpriteSourceData *arg0, u16 *arg1);

void ovl_25_func_800B9D4C(void) {
    u16 sp10[3];
    ObjectState *temp_a0;
    s16 temp_v1;
    s8 var_a1;

    if (D_800BFE60 > 0x28000) {
        var_a1 = 0;
    } else if (D_800BFE60 > 0x1E000) {
        var_a1 = 1;
    } else if (D_800BFE60 > 0x14000) {
        var_a1 = 2;
    } else if (D_800BFE60 > 0xA000) {
        var_a1 = 3;
    } else {
        var_a1 = 4;
    }
    func_80015840((ObjectState *) D_800C02DC, var_a1);
    temp_a0 = (ObjectState *) D_800C02DC - 0x93;
    sp10[0] = (*(u16 *) ((u8 *) temp_a0 + 0x20)) - 0x64;
    sp10[1] = (*(u16 *) ((u8 *) temp_a0 + 0x22)) - 0xDC;
    sp10[2] = (*(u16 *) ((u8 *) temp_a0 + 0x24)) + 0xDC;
    temp_v1 = *(s16 *) ((u8 *) temp_a0 + 0x12);
    if ((temp_v1 != 8) && (temp_v1 != 0)) {
        ovl_25_func_800BA5BC((SpriteSourceData *) D_800C02DC, sp10);
    }
}
