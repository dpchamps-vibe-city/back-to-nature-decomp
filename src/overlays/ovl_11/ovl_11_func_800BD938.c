#include "common.h"
#include "game_types.h"
#include "cd_loader.h"

void ovl_11_func_800DD248(u16 *arg0, u16 *arg1, u32 arg2);

void ovl_11_func_800BD938(void) {
    u16 *temp_v0;

    CdReadDiscSectorsBlocking(0, 0x03D57800, 0x800, 0, D_8005E3B0 + 0x4290);
    temp_v0 = D_8005E3B0 + 0x4290;
    D_801287F8 = temp_v0;
    ovl_11_func_800DD248((u16 *) &D_80070EC2, temp_v0, 0xC0U);
    ovl_11_func_800DD248((u16 *) ((&D_80070EC2 + 0x60)), D_8005E3B0 + 0x4410, 0xC0U);
    ovl_11_func_800DD248((u16 *) ((&D_80070EC2 - 0x60)), D_8005E3B0 + 0x4590, 0xC0U);
}
