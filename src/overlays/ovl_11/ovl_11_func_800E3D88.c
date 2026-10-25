#include "common.h"
#include "game_flags.h"
typedef struct {
    char pad_0[0x2];
    s16 unk2;
} ReconPointee0View;

extern ReconPointee0View *D_8006C868;

s32 ovl_11_func_800E3D88(void) {
    s32 callRet1;
    callRet1 = GetGameFlag(2);
    if (callRet1 == 0) {
        return 0;
    } else {
        return D_8006C868->unk2;
    }
}
