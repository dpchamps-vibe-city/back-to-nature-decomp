#include "common.h"
#include "game_types.h"
#include "cd_loader.h"
#include "ovl11_block_copy.h"

void ovl_11_func_800BD938(void) {
    u16 *temp_v0;

    CdReadDiscSectorsBlocking(0, 0x03D57800, 0x800, 0, gGraphicsHeapBase + 0x4290);
    temp_v0 = gGraphicsHeapBase + 0x4290;
    D_801287F8 = temp_v0;
    Ovl11CopyHalfwordBlock((u16 *) &D_80070EC2, temp_v0, 0xC0U);
    Ovl11CopyHalfwordBlock((u16 *) ((&D_80070EC2 + 0x60)), gGraphicsHeapBase + 0x4410, 0xC0U);
    Ovl11CopyHalfwordBlock((u16 *) ((&D_80070EC2 - 0x60)), gGraphicsHeapBase + 0x4590, 0xC0U);
}
