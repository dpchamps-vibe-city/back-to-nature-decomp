#include "common.h"
#include "game_types.h"

void ovl_11_func_800D6F78(s16 arg0, s16 arg1, s32 *arg2, s16 arg3, s16 arg4, s32 arg5);

void ovl_11_func_800C9F20(Ovl11C7010Object *arg0, s32 arg1, s32 *arg2) {
    s16 var_t1;
    s32 temp_a1;
    s32 four;    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_a0;
    s32 var_t0;
    s32 temp_v0;
    u16 *temp_a0;

    temp_v1 = arg0->unk6C;
    if (temp_v1 & 0x4000) {
        return;
    }
    if (temp_v1 & 0x8000) {
        var_t1 = arg0->itemIndex;
        var_t0 = 0;
        if (!(temp_v1 & 0x18000000)) {
            var_t1 = *(s16 *) ((u8 *) arg0 + 0xF0);
        }
        if (temp_v1 & 0x10000000) {
            var_t0 = 8;
        }
        temp_v0 = arg0->unk3C;
        four = 4;
        temp_a1 = temp_v0 & 0xFFFF;
        temp_v1_2 = temp_v0;
        temp_v0 = four;
        if (temp_a1 == temp_v0) {
            var_t0 = 0x10;
        }
        if ((temp_a1 == 3) || (temp_a1 == 0x31)) {
            var_t0 = 0x18;
        }
        var_a0 = arg0->unk38;
        if (((u16) temp_v1_2 == 0x15) || ((u16) temp_v1_2 == 0x32)) {
            var_a0 = 0;
        }
        temp_a0 = &D_8012315C[var_a0 * 2 + var_t0];
        ovl_11_func_800D6F78(var_t1, 0, (s32 *) (D_8005E3C0->field_120 + ((arg1 >> 2) * 4)), (s16) ((*(u16 *) ((u8 *) arg2 + 0)) + temp_a0[0]), (s16) ((*(u16 *) ((u8 *) arg2 + 4)) + temp_a0[1]), 2);
    }
}
