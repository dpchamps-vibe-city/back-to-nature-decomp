#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

Ovl11D124Entry *ovl_11_func_800DAF60(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800D5868(s16 arg0);
s32 ovl_11_func_800D5E00(u16 a, u16 b);
s32 ovl_11_func_800D5E64(u16 a, u16 b);
s32 ovl_11_func_800D5ED4(u16 a, u16 b);
void ovl_11_func_800F3898(u16 arg0, u16 arg1);
void ovl_11_func_800DA390(Ovl11GridResetEntry *arg0);
s32 ovl_11_func_800DAC08(u16 arg0, s16 arg1, s16 arg2, s16 arg3);

s32 ovl_11_func_800D8C6C(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s32 sp10;
    Ovl11D124Entry *temp_v0;

    u16 temp_s0;
    u8 var_s2;

    sp10 = 0;
    temp_v0 = ovl_11_func_800DAF60((s32) arg0, arg1, arg2);
    if (temp_v0 == NULL) {
        return 0;
    }
    {
        temp_s0 = (u16) temp_v0->unk0;
        var_s2 = 0;
        if (ovl_11_func_800D5868(temp_v0->unk0) == 1) {
            var_s2 = temp_v0->unk4;
        }
        if ((ovl_11_func_800D5E00(temp_s0, (u16) var_s2) == 1) && ((u32) arg4 >= ovl_11_func_800D5E64(temp_s0, (u16) var_s2)) && ((u32) arg3 >= ovl_11_func_800D5ED4(temp_s0, (u16) var_s2))) {
            switch (temp_s0) {                      /* irregular */
            case 0x3D:
                ovl_11_func_800F3898(2U, 0U);
                ovl_11_func_800DA390((void *) temp_v0);
                ovl_11_func_800DA390((void *)&gMainAreaGrid[arg2 + 1][arg1]);
                ovl_11_func_800DA390((void *)&gMainAreaGrid[arg2][arg1 + 1]);
                ovl_11_func_800DA390((void *)&gMainAreaGrid[arg2 + 1][arg1 + 1]);
                ovl_11_func_800DAC08(0x3DU, arg0, arg1 + 1, arg2 + 1);
                break;
            case 0x172:
                ovl_11_func_800F3898(2U, 0U);
                ovl_11_func_800DA390((void *) temp_v0);
                ovl_11_func_800DA390((void *)&gMainAreaGrid[arg2 + 1][arg1]);
                ovl_11_func_800DA390((void *)&gMainAreaGrid[arg2][arg1 - 1]);
                ovl_11_func_800DA390((void *)&gMainAreaGrid[arg2 + 1][arg1 - 1]);
                ovl_11_func_800DAC08(0x3DU, arg0, arg1, arg2 + 1);
                break;
            case 0x174:
                ovl_11_func_800F3898(2U, 0U);
                ovl_11_func_800DA390((void *) temp_v0);
                ovl_11_func_800DA390((void *)&gMainAreaGrid[arg2 - 1][arg1]);
                ovl_11_func_800DA390((void *)&gMainAreaGrid[arg2][arg1 + 1]);
                ovl_11_func_800DA390((void *)&gMainAreaGrid[arg2 - 1][arg1 + 1]);
                ovl_11_func_800DAC08(0x3DU, arg0, arg1 + 1, arg2);
                break;
            case 0x173:
                ovl_11_func_800F3898(2U, 0U);
                ovl_11_func_800DA390((void *) temp_v0);
                ovl_11_func_800DA390((void *)&gMainAreaGrid[arg2 - 1][arg1]);
                ovl_11_func_800DA390((void *)&gMainAreaGrid[arg2 - 1][arg1 - 1]);
                ovl_11_func_800DA390((void *)&gMainAreaGrid[arg2][arg1 - 1]);
                ovl_11_func_800DAC08(0x3DU, arg0, arg1, arg2);
                break;
            case 0x3E:
                ovl_11_func_800F3898(1U, 0U);
                ovl_11_func_800DA390((void *) temp_v0);
                ovl_11_func_800DAC08(0x3EU, arg0, arg1, arg2);
                break;
            default:
                ovl_11_func_800DA390((void *) temp_v0);
                break;
            }
            sp10 = 1;
        }
    }
    return sp10;
}
