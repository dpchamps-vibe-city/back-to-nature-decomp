#include "common.h"
#include "ovl15_save_checksums.h"
#include "game_types.h"
#include "psyq/memory.h"
#include "psyq/strings.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
void ovl_15_func_80134A34(s16 arg0);
void ovl_15_func_80134C48(void);
void ovl_15_func_801359FC(void);
void ovl_15_func_80137544(char *arg0, s32 arg1);
s8 *ovl_15_func_80134770(s16 arg0);

void ovl_15_func_8012FAE0(void) {
    s32 temp_a1;
    u32 *icons;
    u32 paletteOffset;

    temp_a1 = gActiveRenderContext->field_D8;
    icons = D_801375B4[D_8013758E];
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14,
                 (s32)(D_8005346A + D_80054BBC[0]));
    ovl_15_func_80134A34(D_8013758E);
    ovl_15_func_80134C48();
    ovl_15_func_801359FC();
    Ovl15WriteSaveChecksums();
    ovl_15_func_80137544(Ovl15CardFilename, D_8013758E);
    memcpy(&Ovl15CardHeader, D_8012DEBC, 3);
    Ovl15CardHeader.type = 0x13;
    Ovl15CardHeader.blocks = 4;
    ovl_15_func_80134770(D_8013758E);
    memset(Ovl15CardHeader.reserved, 0, sizeof(Ovl15CardHeader.reserved));
    paletteOffset = icons[0] + 0x14;
    Ovl15CardHeader.palette = *(Ovl15IconPalette *)(D_80137828 + paletteOffset);
    memcpy(D_80137830, &Ovl15CardHeader, sizeof(Ovl15CardHeader));
    memcpy(D_801378B0, (void *)(icons[0] + (u32)D_80137828 + 0x40), 0x80);
    memcpy(D_80137930, (void *)(icons[1] + (u32)D_80137828 + 0x40), 0x80);
    memcpy(D_801379B0, (void *)(icons[2] + (u32)D_80137828 + 0x40), 0x80);
    D_80137584 = 0x14;
}
