#include "common.h"
#include "game_types.h"
#include "cd_loader.h"

void ovl_11_func_800BD9D4(s32 arg0) {
    CdReadDiscSectorsBlocking(0, ((arg0 << 1) + arg0 << 11) + 0x3D5A800, 0x1800, 0, ((s32)(&D_8007BFF8)));
}
