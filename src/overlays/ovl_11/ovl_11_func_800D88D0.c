#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

Ovl11D124Entry *ovl_11_func_800DAF60(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800D5868(s16 arg0);
s32 ovl_11_func_800D5D9C(u16 a, u16 b);
s32 ovl_11_func_800D5E64(u16 a, u16 b);
s32 ovl_11_func_800D5ED4(u16 a, u16 b);
void ovl_11_func_800DA390(Ovl11GridResetEntry *arg0);
s32 ovl_11_func_800DAC08(u16 arg0, s16 arg1, s16 arg2, s16 arg3);

s32 ovl_11_func_800D88D0(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s32 sp10;
    Ovl11D124Entry *temp_v0;
    u16 temp_s3;
    u8 var_s1;

    sp10 = 0;
    temp_v0 = ovl_11_func_800DAF60((s32) arg0, arg1, arg2);
    if (temp_v0 == NULL) {
        return 0;
    }
    temp_s3 = (u16) temp_v0->unk0;
    var_s1 = 0;
    if (ovl_11_func_800D5868(temp_v0->unk0) == 1) {
        var_s1 = temp_v0->unk4;
    }
    if ((ovl_11_func_800D5D9C(temp_s3, (u16) var_s1) == 1) && ((u32) arg4 >= ovl_11_func_800D5E64(temp_s3, (u16) var_s1)) && ((u32) arg3 >= ovl_11_func_800D5ED4(temp_s3, (u16) var_s1))) {
        switch (temp_s3) {
        case 0x3B:
        case 0x3C:
            ovl_11_func_800DA390((void *) temp_v0);
            ovl_11_func_800DA390((void *)&D_80071DFC[arg2 + 1][arg1]);
            ovl_11_func_800DA390((void *)&D_80071DFC[arg2][arg1 + 1]);
            ovl_11_func_800DA390((void *)&D_80071DFC[arg2 + 1][arg1 + 1]);
            ovl_11_func_800DAC08(temp_s3, arg0, arg1 + 1, arg2 + 1);
            break;
        case 0x16C:
        case 0x16F:
            ovl_11_func_800DA390((void *) temp_v0);
            ovl_11_func_800DA390((void *)&D_80071DFC[arg2 + 1][arg1]);
            ovl_11_func_800DA390((void *)&D_80071DFC[arg2][arg1 - 1]);
            ovl_11_func_800DA390((void *)&D_80071DFC[arg2 + 1][arg1 - 1]);
            ovl_11_func_800DAC08(temp_s3, arg0, arg1, arg2 + 1);
            break;
        case 0x16E:
        case 0x171:
            ovl_11_func_800DA390((void *) temp_v0);
            ovl_11_func_800DA390((void *)&D_80071DFC[arg2 - 1][arg1]);
            ovl_11_func_800DA390((void *)&D_80071DFC[arg2][arg1 + 1]);
            ovl_11_func_800DA390((void *)&D_80071DFC[arg2 - 1][arg1 + 1]);
            ovl_11_func_800DAC08(temp_s3, arg0, arg1 + 1, arg2);
            break;
        case 0x16D:
        case 0x170:
            ovl_11_func_800DA390((void *) temp_v0);
            ovl_11_func_800DA390((void *)&D_80071DFC[arg2 - 1][arg1]);
            ovl_11_func_800DA390((void *)&D_80071DFC[arg2 - 1][arg1 - 1]);
            ovl_11_func_800DA390((void *)&D_80071DFC[arg2][arg1 - 1]);
            ovl_11_func_800DAC08(temp_s3, arg0, arg1, arg2);
            break;
        case 0x3A:
        case 0x3F:
        case 0x175:
            ovl_11_func_800DAC08(temp_s3, arg0, arg1, arg2);
        default:
            ovl_11_func_800DA390((void *) temp_v0);
            D_80123DFC[arg0](arg1, arg2, 0);
            break;
        }
        sp10 = 1;
    }
    return sp10;
}
