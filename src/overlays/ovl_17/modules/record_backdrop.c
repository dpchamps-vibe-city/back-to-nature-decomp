/*
 * record_backdrop — reconstructed translation unit.
 *
 * Draw two scrolling tile layers, edge markers and table-driven decorations
 * using the record scene display and prop sprite sources.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "ovl17_record_backdrop.h"

static __inline__ void drawTile(s32 ot, u8 *sprite, s16 x) {
    u8 *work = sprite - 0x22C;
    DrawSpriteFrameRects(ot, (s32)sprite, work[0x230], work[0x231], x, 0);
}

void Ovl17DrawRecordBackdrop(s32 scrollQ12) {
    s16 x;
    s16 edgeX;
    s16 edgeX2;
    s16 edgeX3;
    s32 pixels;
    s32 propIndex;
    s32 ot;
    u8 *edgeBase;
    u8 *endBase;

    ot = gActiveRenderContext->field_D8 + 0x58;
    SelectSpriteAnimation((ObjectState *)gOvl17DisplaySpriteSource, 0);
    SelectSpriteAnimationFrame((ObjectState *)gOvl17DisplaySpriteSource, 0);
    /* Signed divisions truncate toward zero, including negative scroll. */
    for (x = scrollQ12 / 4096 - (scrollQ12 / 262144) * 64 - 64; x < 384; x += 64) {
        drawTile(gActiveRenderContext->field_D8 + 0x5C, gOvl17DisplaySpriteSource, x);
    }
    SelectSpriteAnimation((ObjectState *)gOvl17DisplaySpriteSource, 1);
    x = (scrollQ12 / 4096) * 2 - (scrollQ12 / 155648) * 76 - 76;
    for (; x < 396; x += 76) {
        drawTile(gActiveRenderContext->field_D8 + 0x18, gOvl17DisplaySpriteSource, x);
    }
    pixels = scrollQ12 / 4096;
    if ((u32)(pixels + 191) < 383) {
        SelectSpriteAnimation((ObjectState *)gOvl17DisplaySpriteSource, 0);
        SelectSpriteAnimationFrame((ObjectState *)gOvl17DisplaySpriteSource, 1);
        edgeX = pixels + 160;
        edgeX -= 32;
        edgeBase = gOvl17DisplaySpriteSource - 0x22C;
        DrawSpriteFrameRects(ot, (s32)gOvl17DisplaySpriteSource, edgeBase[0x230], edgeBase[0x231], edgeX, 0);
        SelectSpriteAnimation((ObjectState *)gOvl17DisplaySpriteSource, 2);
        edgeX2 = pixels + 224;
        edgeX2 -= 32;
        DrawSpriteFrameRects(ot, (s32)gOvl17DisplaySpriteSource, edgeBase[0x230], edgeBase[0x231], edgeX2, 0);
    }
    if ((u32)(pixels - 1857) < 383) {
        SelectSpriteAnimation((ObjectState *)gOvl17DisplaySpriteSource, 0);
        SelectSpriteAnimationFrame((ObjectState *)gOvl17DisplaySpriteSource, 1);
        edgeX3 = pixels - 1888;
        edgeX3 -= 32;
        endBase = gOvl17DisplaySpriteSource - 0x22C;
        DrawSpriteFrameRects(ot, (s32)gOvl17DisplaySpriteSource, endBase[0x230], endBase[0x231], edgeX3, 0);
    }
    for (propIndex = 0; gOvl17BackdropProps[propIndex].spriteKind != 255; propIndex++) {
        if ((scrollQ12 / 4096) * 2 + HWD0 > gOvl17BackdropProps[propIndex].xQ12 / 4096 &&
            (scrollQ12 / 4096) * 2 - HWD0 < gOvl17BackdropProps[propIndex].xQ12 / 4096) {
            x = HWD0 / 2 + (scrollQ12 / 4096) * 2 - gOvl17BackdropProps[propIndex].xQ12 / 4096;
            SelectSpriteAnimation((ObjectState *)&gOvl17SceneSpriteSource, gOvl17BackdropPropAnimations[gOvl17BackdropProps[propIndex].spriteKind]);
            SelectSpriteAnimationFrame((ObjectState *)&gOvl17SceneSpriteSource, x < HWD0 / 2);
            DrawSpriteFrameQuads(gActiveRenderContext->field_D8 + 0x14, (s32)&gOvl17SceneSpriteSource,
                          gOvl17SceneSpriteSource.field_4, gOvl17SceneSpriteSource.field_5,
                          x, (s16)VWD0, 0x1000, 0, 0);
        }
    }
}
