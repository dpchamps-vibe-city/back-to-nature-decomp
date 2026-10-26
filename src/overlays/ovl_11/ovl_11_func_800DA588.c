#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

extern SpriteSourceData gSpriteSourceBankBase;

void ovl_11_func_800DA588(s32 arg0) {
    SpriteSourceData *out;
    s32 n;
    s32 i;
    s32 off;

    n = *gOvl11SpriteRecordSets[arg0];
    out = &gSpriteSourceBankBase;
    if (n >= 0x2D) {
        n = 0x2C;
    }
    i = 0;
    if (n > 0) {
        do {
            off = i * 4;
            i += 1;
            off = (s32)((u8 *)gOvl11SpriteRecordSets[arg0] + off);
            InitSpriteSourceData(out, (SpriteDataHeader *)(((s32 *)off)[1] + (s32)gLoadedSpriteDataBlob));
            out += 1;
        } while (i < n);
    }
}
