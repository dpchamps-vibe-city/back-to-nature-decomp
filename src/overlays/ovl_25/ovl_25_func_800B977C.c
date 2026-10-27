#include "common.h"
#include "random.h"
#include "game_types.h"

s32 ovl_25_func_800B977C(Ovl25RecordIdView *record, s16 choice) {
    s32 callRet3;
    s32 idx;
    callRet3 = Rand(0x80);
    idx = record->id - 1;
    return callRet3 < D_800BCCA0[idx * 5 + choice] ^ 1;
}
