/*
 * Scrolling sprite grid, exe:8002238C..80022580.
 *
 * Two renderer entrypoints share one callback-driven grid walk. Its private
 * tick and frame-index table produce a diagonal, repeating 24-pixel pattern.
 * Sprite storage is shared with other drawing code, not owned exclusively by
 * this module. See sprite_grid.h for packet-cursor and reentrancy constraints,
 * and notes/file-groupings.md for membership and boundary evidence.
 */
#include "common.h"
#include "game_types.h"
#include "sprite_grid.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

/* Original-byte GP access identifies this unit as the tick's owner. */
s32 gSpriteGridTick;

/* Helpers retain external linkage; only the three grid entrypoints are
 * published. */
void DrawSpriteGridCell(s32 ot, SpriteSourceData *source, s16 frame, s32 x, s32 y);
void DrawSpriteGridCellInContext(s32 ot, SpriteSourceData *source, s16 frame, s32 x, s32 y);

void DrawScrollingSpriteGrid(s32 orderingTable) {
    VisitScrollingSpriteGrid(orderingTable, DrawSpriteGridCell);
}

void DrawScrollingSpriteGridInContext(s32 orderingTable) {
    VisitScrollingSpriteGrid(orderingTable, DrawSpriteGridCellInContext);
}

void VisitScrollingSpriteGrid(s32 orderingTable, SpriteGridCallback visitor) {
    SpriteGridCallback drawCell;
    s32 scrollOffset;
    s32 cellY;
    u32 nextTick;
    s32 rowY;
    s32 columnX;
    s32 frameSlot;

    InitSpriteSourceData((SpriteSourceData *)&gSharedSpriteSource,
                  (SpriteDataHeader *)&gSharedSpriteData);

    nextTick = (u32)gSpriteGridTick + 1;
    scrollOffset = (nextTick >> 2) - ((nextTick / 96) * 24);
    gSpriteGridTick = (s32)nextTick;
    frameSlot = 0;
    drawCell = visitor;
    rowY = 0;

    do {
        columnX = -12;
        cellY = rowY - scrollOffset;
        do {
            frameSlot++;
            frameSlot = frameSlot & 3;
            drawCell(orderingTable,
                     (SpriteSourceData *)&gSharedSpriteSource,
                     gSpriteGridFrameIndices[frameSlot],
                     columnX - scrollOffset,
                     cellY);
            columnX += 24;
        } while (columnX < 344);
        rowY += 24;
    } while ((u32)rowY < 264);
}

void DrawSpriteGridCell(s32 ot, SpriteSourceData *source, s16 frame, s32 x, s32 y) {
    DrawSpriteFrameRects(ot, (s32)source, 29, frame & 0xFF, (s16)x, (s16)y);
}

void DrawSpriteGridCellInContext(s32 ot, SpriteSourceData *source, s16 frame, s32 x, s32 y) {
    BuildSpriteRectPacketsFromAddresses(ot, (s32)&gActiveRenderContext->field_118,
                   (s32)&gActiveRenderContext->field_118, (s32)source,
                   29, frame & 0xFF, (s16)x, (s16)y);
}
