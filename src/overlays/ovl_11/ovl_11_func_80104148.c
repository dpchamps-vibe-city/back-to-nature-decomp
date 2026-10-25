#include "common.h"

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);

void ovl_11_func_80104148(s32 arg0) {
    s16 temp_s2;
    s16 temp_s7;
    s16 temp_v0;
    s16 var_s1;
    s32 var_t0;
    s32 mode;
    s32 three;
    register s16 x;
    s32 *var_a1;
    s16 *buffer;
    s32 temp_s6;
    Ovl11CardView *temp_v1;

    var_t0 = 2;
    arg0 = (s16) arg0;
    temp_s6 = gActiveRenderContext->field_D8 + 0x58;
    temp_v1 = (Ovl11CardView *) D_8006C838;
    mode = temp_v1->selector;
    three = 3;
    if (mode != three) {
        var_t0 = mode;
    }
    var_s1 = temp_v1->cards[var_t0][arg0][0];
    temp_s2 = temp_v1->cards[var_t0][arg0][5];
    temp_s7 = D_8012CF10[arg0];
    temp_v0 = ((arg0 + 1) * 0xE) + 0x32;
    if (mode == three && arg0 == 2) {
        var_s1 = -1;
    }
    D_8012CF30_glyph.glyph = arg0 < 2 ? arg0 + 0x79 : arg0 + 0x8F;
    D_8012CF30_glyph.terminator = 0xFFFF;
    x = 0x2B;
    func_80017B3C(temp_s6, (s32) &D_8012CF30_glyph.glyph, x, (s32) temp_v0);
    if (var_s1 != -1) {
        var_a1 = (s32 *) (D_800556A8 + D_80127430[var_s1][0]);
        x += 0x12;
    } else {
        var_a1 = &D_80075AD8;
        x += 0x12;
    }
    func_80017B3C(temp_s6, (s32) var_a1, x, (s32) temp_v0);
    x += 0x51;
    func_80017B3C(temp_s6, (s32) (D_80051BF4 + *gTextBankOffsets), x, (s32) temp_v0);
    x += 0x24;
    buffer = &D_8012CF30_glyph.glyph;
    *(u16 *)func_8001A970((s32) temp_s2, buffer, 2) = 0xFFFF;
    func_80017B3C(temp_s6, (s32) buffer, x, (s32) temp_v0);
    x += 0x1B;
    *(u16 *)func_8001A970((s32) temp_s7, buffer, 2) = 0xFFFF;
    func_80017B3C(temp_s6, (s32) buffer, x, (s32) temp_v0);
    x += 0x12;
    func_80017B3C(temp_s6, (s32) (D_80051BF4 + 0xA + *gTextBankOffsets), x, (s32) temp_v0);
}
