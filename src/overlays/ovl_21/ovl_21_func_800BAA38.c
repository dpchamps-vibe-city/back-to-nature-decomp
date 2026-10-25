#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

void ovl_21_func_800BAA38(s32 arg0) {
    s32 i;
    s32 j;
    struct SpriteSourceData *sprite;
    u16 *record;
    void (*updateFlags)(u16 *, s32);

    if (arg0 != 0) {
        updateFlags = AddSpriteSourceFlags;
    } else {
        updateFlags = ClearSpriteSourceFlags;
    }
    updateFlags(&D_800C0ACC.field_0, 4);
    updateFlags(&(&D_800C0ACC)[1].field_0, 4);
    updateFlags(&(&D_800C0ACC)[2].field_0, 4);
    sprite = &D_800C0ACC + 3;
    for (i = 0; i < 13; i++) {
        updateFlags(&sprite->field_0, 4);
        sprite++;
    }
    for (i = 0; i < 6; i++) {
        record = (u16 *) (D_800C0458 + i * 0x108);
        for (j = 0; j < 3; j++) {
            updateFlags((u16 *) ((u8 *) record + j * 0x4C + 0x40), 4);
        }
    }
}
