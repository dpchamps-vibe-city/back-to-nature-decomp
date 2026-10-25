#include "common.h"
#include "game_types.h"
#include "cd_loader.h"

void func_8001719C(u8 *arg0);

void ovl_11_func_800BDA20(void) {
    CdReadDiscSectorsBlocking(0, 0x3D51000, 0x6800, 0, D_8005E3B0 + 0x4290);
    func_8001719C(((void *)(D_8005E3B0 + 0x4290)));
}
