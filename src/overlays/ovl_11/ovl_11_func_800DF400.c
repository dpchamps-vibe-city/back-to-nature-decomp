#include "common.h"

#include "game_types.h"
#include "random.h"

s32 ovl_11_func_800DF010(void *arg0, s32 arg1);

s32 ovl_11_func_800DF400(UnkStruct800DF4F0 *arg0) {
    s32 state;
    s32 limit;

    if ((u32)(D_80070CF8_halfword - 6) >= 15) {
        state = 15;
    } else if (*(s16 *)((u8 *)arg0 + 0x26) == 15) {
        state = 16;
    } else {
        limit = 4;
        if (*(s32 *)((u8 *)arg0 + 0x34) & 0x2000) {
            limit = 9;
        }
        switch ((s32)Rand(limit)) {
        case 0:
            state = 18;
            break;
        case 1:
            state = 14;
            break;
        case 2:
            state = 15;
            break;
        case 3:
            if (*(u16 *)((u8 *)arg0 + 0xB2) != 0) {
                state = 14;
            } else {
                state = 17;
            }
            break;
        default:
            state = 18;
            break;
        }
    }
    if (state != *(s16 *)((u8 *)arg0 + 0x26)) {
        ovl_11_func_800DF010(arg0, state);
    }
    return 0;
}
