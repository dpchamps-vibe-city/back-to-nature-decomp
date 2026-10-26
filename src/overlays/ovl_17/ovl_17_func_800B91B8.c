#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"

void ovl_17_func_800B91B8(void) {
    s16 y;
    s16 i;
    s32 screenX;
    s16 mirror;
    u8 *base;
    s16 *yFields;

    base = gOvl17RecordWorkBase;
    i = 0;
    do {
        yFields = (s16 *)(base + 0x2DC);
        y = yFields[i * 4];
        if (y != -1) {
            mirror = (i - (s16)(i / 12) * 12) >= 6;
            screenX = 0xA0 -
                (((Ovl17QueueView *)base)->entries[i].word -
                 *(s32 *)(base + 0x24)) / 0x1000;
            DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x54,
                         (s32)(base + 0x22C), 3, mirror, (s16)screenX, y);
        }
        i++;
    } while (i < 90);
}
