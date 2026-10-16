#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800C9D64(void);
void ovl_11_func_800CE034(Ovl11CE034Object *arg0, Ovl11CE034Position *arg1, s32 arg2);
s32 ovl_11_func_8011E1E8(s16 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800D759C(s16 arg0, s16 arg1, s16 arg2);

void ovl_11_func_800C55FC(Ovl11CE034Object *arg0) {
    Ovl11CE034Position sp10;
    s16 sp20;
    s16 sp22;
    s32 temp_s0;
    Ovl11D124Entry *var_s0;

    temp_s0 = ovl_11_func_800C9D64();
    ovl_11_func_800CE034(arg0, &sp10, 1);
    if (temp_s0 != 2) {
        D_80123234[temp_s0](&sp10, &sp20, &sp22);
        if (temp_s0 != 0) {
            var_s0 = &D_80074124[sp22][sp20];
            ovl_11_func_8011E1E8(sp20, sp22, 0);
        } else {
            var_s0 = &D_80071DFC[sp22][sp20];
            ovl_11_func_800D759C(sp20, sp22, 0);
        }
        var_s0->unk0 = *(u16 *) ((u8 *) arg0 + 0x8A);
        if ((u16)var_s0->unk0 == 0x13B) {
            var_s0->unk0 = 0x3AU;
        }
        if ((u16)var_s0->unk0 == 0x13C) {
            var_s0->unk0 = 0x3EU;
        }
        if ((u16)var_s0->unk0 == 0x13D) {
            var_s0->unk0 = 0x3FU;
        }
        var_s0->unk4 = 0;
        var_s0->unk5 = 0;
        var_s0->unk6 = 0;
        var_s0->unk2 = 0x167;
    }
}
