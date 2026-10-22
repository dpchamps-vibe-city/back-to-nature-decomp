#include "common.h"
#include "game_types.h"

u16 ovl_11_func_800D062C(Ovl11RankPair *arg0) {
    s32 ovl_11_func_800D0600(s32 a, s32 b) {
        if (a == b) {
            return 0;
        }
        if (a < b) {
            return 1;
        }
        return 2;
    }
    s32 temp_s1;
    s32 second;

    temp_s1 = ovl_11_func_800D0600(arg0->unk38, arg0->unk58);
    second = ovl_11_func_800D0600(arg0->unk40, arg0->unk60);
    return D_80123A00[temp_s1 * 3 + second];
}
