#include "common.h"
#include "psyq/memory.h"

void ovl_15_func_80134A34(s16 arg0) {
    D_801376E0_headers[arg0].words = D_80070CF0_headerSource.words;
    memcpy(&D_801376E0_headers[arg0].name_8, &D_80070CF0_headerSource.name_28, sizeof(D_80070CF0_headerSource.name_28));
    memcpy(&D_801376E0_headers[arg0].name_1A, &D_80070CF0_headerSource.name_D10, sizeof(D_80070CF0_headerSource.name_D10));
    memcpy(&D_801376E0_headers[arg0].fields_2C, (u8 *)&D_80070CF0_headerSource - 0x44B8, sizeof(D_801376E0_headers[arg0].fields_2C));
    D_801376E0_headers[arg0].field_34 = 0x14;
    memset(gOvl15TransferBuffer, 0, 0x9690);
    memcpy(gOvl15TransferBuffer + 0x200, &D_801376E0_headers[arg0], sizeof(D_801376E0_headers[arg0]));
}
