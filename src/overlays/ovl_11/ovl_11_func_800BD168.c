#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

extern u8 D_80071C60[];

void ovl_11_func_800BD168(void) {
    s32 *header;

    header = (s32 *)&D_8007AFF0;
    InitSpriteSourceData((SpriteSourceData *)D_80071C60,
                  (SpriteDataHeader *)header[1]);
    SetSpriteSourceRecordAddress((SomeStruct *)D_80071C60, header[1] + 0xBE08);
}
