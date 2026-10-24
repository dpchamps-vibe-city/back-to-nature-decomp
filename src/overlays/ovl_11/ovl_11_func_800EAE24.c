#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


u8 *ovl_11_func_800EFF04(s32 arg0, s32 arg1, s32 *arg2);
s32 ovl_11_func_800E5B84(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E65AC(s16 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800D0408(s16 arg0, Recon800D0408A1View *arg1, s32 arg2);

s32 ovl_11_func_800EAE24(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;
    s32 temp_v1;
    u8 *temp_s3;

    temp_s3 = ovl_11_func_800EFF04(0x23, (s32) arg0, NULL);
    switch (arg3) {
    case 0:
    case 1:
        ovl_11_func_800E5B84(arg0, arg1, arg2, arg3);
        break;
    case 2:
        temp_v1 = *(s32 *) ((u8 *) temp_s3 + 8);
        if (temp_v1 < 0) {
            ovl_11_func_800E5B84(arg0, arg1, 0, 1);
        } else {
            temp_v0 = *(s32 *) ((u8 *) temp_s3 + 0);
            if (temp_v0 < 0) {
                ovl_11_func_800E5B84(arg0, arg1, 1, 1);
            } else if (temp_v1 > 0) {
                ovl_11_func_800E5B84(arg0, arg1, 2, 1);
            } else if (temp_v0 > 0) {
                ovl_11_func_800E5B84(arg0, arg1, 3, 1);
            }
        }
        break;
    case 3:
        ovl_11_func_800E65AC(arg0, 0, arg1, 0);
        break;
    }
    ovl_11_func_800D0408(4, (Recon800D0408A1View *) temp_s3, 0);
    return 1;
}
