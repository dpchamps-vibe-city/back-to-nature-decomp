#include "common.h"
#include "game_types.h"

u32 Rand(s32 arg0);
void func_80015840(ObjectState *obj, u8 arg1);
void func_80015BF0(s32 arg0, SpriteSourceData *arg1, s32 arg2, s32 arg3);

void ovl_27_func_800B9B98(void) {
    s32 var;
    s16 q;
    s16 r;

    var = (u16)D_800C4A30 + 1;
    q = (s16)var / 1800;
    D_800C4A30 = var - q * 1800;
    func_80015BF0(D_8005E3C0->field_D8 + 0x8C, (SpriteSourceData *)D_800C4AE0, 0x33, 0xB6);
    if (D_800C4A30 >= 0x3C) {
        D_800C4A30 = 0;
        r = Rand(3) + 0x13;
        if (r == 0x13) {
            D_800C4A30 = 0x1E;
        }
        func_80015840((ObjectState *)D_800C4AE0, r);
    }
}
