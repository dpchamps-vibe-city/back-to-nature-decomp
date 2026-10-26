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

extern u16 D_801231FC[];

void ovl_11_func_800CCFF8(Recon_ovl_11_func_800CCFF8_A0View *arg0, s32 arg1, s32 arg2, s32 arg3) {
    SpriteSourceData *temp_s1;
    s16 temp_v0;

    temp_v0 = arg0->unk3C - 0x3B;
    if (arg1 != 0) {
        Ovl11LoadLargeIndexedSpriteBank(temp_v0);
    }
    temp_s1 = (SpriteSourceData *) ((u8 *) arg0 + 0x260);
    InitSpriteSourceData(temp_s1, (SpriteDataHeader *) D_8007BFF8);
    SetSpriteSourceRecordAddress((SomeStruct *) temp_s1, (s32) &D_8007BFF8[D_801231FC[temp_v0]]);
    SelectSpriteAnimation((ObjectState *) temp_s1, 0);
}
