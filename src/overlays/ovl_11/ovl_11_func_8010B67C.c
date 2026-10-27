#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/libgs.h"
#include "ovl11_area_resource_load.h"

void ovl_11_func_8010B67C(void) {
    char *base;
    char *far0;
    char *far1;
    char *state;
    s32 var_a0;

    base = (char *)&gStageMode2Record;
    *(s32 *)(base + 0x34) |= 0x8000000;
    far0 = (char *)&D_8007AFF0;
    if ((*(s16 *)(far0 + 0x25476) != 3) && (*(s16 *)(far0 + 0x25476) != 1)) {
        *(s32 *)(base + 0x8C) = 0;
        return;
    }
    switch (*(u16 *)(base + 0)) {
    case 0x15E:
        var_a0 = 1;
        goto block_8;
    case 0x15F:
        var_a0 = 0;
block_8:
        Ovl11LoadSceneSpriteData(var_a0);
        InitSpriteSourceData((SpriteSourceData *)(base + 0x78), (SpriteDataHeader *)&D_8007AFF8);
        far1 = (char *)&D_8007AFF8;
        state = far1 + 0x1FFF8;
        if (*(s16 *)(state + 0x5476) == 3) {
            SetSpriteSourceTextureOffsets((Struct_800154CC *)(base + 0x78), -0x100, 0, 0, 0);
        }
        return;
    default:
        *(s32 *)(base + 0x34) &= 0xF7FFFFFF;
        *(s32 *)(base + 0x8C) = 0;
        break;
    }
}
