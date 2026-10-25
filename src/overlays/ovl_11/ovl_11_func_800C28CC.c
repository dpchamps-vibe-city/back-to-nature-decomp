#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

s32 ovl_11_func_800C2884(s16 key, u32 *table);

s32 ovl_11_func_800C28CC(s16 arg0, SpriteSourceData *arg1) {
    s32 group;
    s32 day;
    s32 first;
    s32 index;
    u32 *table;
    u32 *fallback;

    table = (u32 *) &D_8009CBF8;
    fallback = D_8008F7F8_words;
    group = ((struct Ovl11SpriteWork *) &D_8007AFF0)->header[6];
    index = ovl_11_func_800C2884(arg0, table);
    first = index;
    if (index == -1) {
        index = ovl_11_func_800C2884(arg0, fallback);
        if (index != first) {
            group = (s16) ((s16) ((struct Ovl11SpriteSelectors *) D_8006C838)->hour / 6);
            if (group >= 3) {
                group = 2;
            }
            table = fallback;
            day = (s16) ((struct Ovl11SpriteSelectors *) D_8006C838)->day;
            group += day * 3 - (day / 7) * 21;
            group += 14;
        } else {
            return 0;
        }
    }
    if (group == -1) {
        while (1) {
        }
    }
    D_80128B74 = group;
    D_80128B78 = index + 1;
    if (group == 0x3E6) {
        group = ((struct Ovl11SpriteSelectors *) D_8006C838)->season * 3 + 35;
        group += ((struct Ovl11SpriteSelectors *) D_8006C838)->variant;
    }
    InitSpriteSourceData(arg1, (SpriteDataHeader *) ((u8 *) table + D_80125DC0[group][index + 1]));
    return 1;
}
