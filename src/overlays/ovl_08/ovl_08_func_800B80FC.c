#include "common.h"
#include "master_volume.h"

extern s32 D_800B8500;

void ovl_08_func_800B80FC(void) {
    if (IsSoundTransferComplete() != 0) {
        D_800B8500 += 1;
    }
}
