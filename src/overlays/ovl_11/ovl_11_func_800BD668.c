#include "common.h"
#include "game_types.h"
#include "cd_loader.h"

void ovl_11_func_800BD668(void) {
    CdReadDiscSectorsBlocking(0, 0x74800, 0xA800, 0, D_8005E3B0 + 0x4290);
}
