#include "common.h"
#include "game_types.h"
#include "psyq/memory.h"

s32 ovl_11_func_800CADBC(Ovl11Func800CADBCArg0 *arg0, s32 arg1);

s32 ovl_11_func_800CAE64(Ovl11Func800CADBCArg0 *arg0, s32 arg1, Ovl11Func800CADBCArg0 *arg2, s32 arg3) {
    Ovl11Func800CADBCArg0 *temp_v0;
    Ovl11Func800CADBCArg0 *temp_v0_3;
    Ovl11Func800CADBCArg0 tmp;
    s32 var_a3;

    if ((arg0->unk0 != 0) || (arg2->unk0 != 0)) {
        switch (arg3) {
        case 0:
            tmp = *arg0;
            arg0->unk0 = 0;
            arg0->unk2 = 0;
            arg0->unk4 = 0;
            ovl_11_func_800CADBC(arg0, arg1);
            temp_v0 = (Ovl11Func800CADBCArg0 *) (arg1 * 6 + (s32) arg0);
            temp_v0[-1] = *arg2;
            ovl_11_func_800CADBC(arg0, arg1);
            *arg2 = tmp;
            break;
        case 1:
            var_a3 = arg1 - 1;
            while ((var_a3 >= 0) && (arg0[var_a3].unk0 == 0)) {
                var_a3 -= 1;
            }
            temp_v0_3 = (Ovl11Func800CADBCArg0 *) (var_a3 * 6 + (s32) arg0);
            tmp = *temp_v0_3;
            temp_v0_3->unk0 = 0;
            temp_v0_3->unk2 = 0;
            temp_v0_3->unk4 = 0;
            memmove((u8 *) (arg0 + 1), (u8 *) arg0, var_a3 * 6);
            *arg0 = *arg2;
            *arg2 = tmp;
            ovl_11_func_800CADBC(arg0, arg1);
            break;
        }
        return -1;
    }
    return -1;
}
