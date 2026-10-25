#include "common.h"
#include "random.h"
typedef struct {
    s16 unk0;
} Recon800B977CA0View;

extern s16 D_800BCCA0[];

s32 ovl_25_func_800B977C(Recon800B977CA0View *arg0, s16 arg1) {
    s32 callRet3;
    s32 idx;
    callRet3 = Rand(0x80);
    idx = arg0->unk0 - 1;
    return callRet3 < D_800BCCA0[idx * 5 + arg1] ^ 1;
}
