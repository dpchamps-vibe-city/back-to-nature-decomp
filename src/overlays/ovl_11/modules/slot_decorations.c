/* Menu-grid decorations, ovl_11:800F6F40–800F71DC.
 * These services reuse the resident shared sprite source: selected-cell
 * marker, empty-cell fallback, quantity label, and a cell-relative cursor.
 * They submit packets but do not advance selection or alter item records.
 * Membership and evidence-limited boundaries: notes/file-groupings.md. */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "ovl11_slot_decorations.h"

void func_800248B0(s32 orderingTable, s16 x, s16 y);

void Ovl11DrawSlotCursor(s32 orderingTable, s16 x, s16 y) {
    func_800248B0(orderingTable, (s16)(x + 12), (s16)(y + 24));
}

void Ovl11DrawSelectedSlotMarker(s32 orderingTable, s16 x, s16 y) {
    SelectSpriteAnimation((ObjectState *)&gSharedSpriteSource, 0xB);
    SelectSpriteAnimationFrame((ObjectState *)&gSharedSpriteSource, 0);
    DrawSpriteFrameQuads(orderingTable, (s32)&gSharedSpriteSource,
        gSharedSpriteSource.field_4, gSharedSpriteSource.field_5,
        x, y, 0x1000, 0, 0);
}

void Ovl11DrawEmptySlot(s32 orderingTable, s16 x, s16 y) {
    SelectSpriteAnimation((ObjectState *)&gSharedSpriteSource, 0xB);
    SelectSpriteAnimationFrame((ObjectState *)&gSharedSpriteSource, 2);
    DrawSpriteFrameRects(orderingTable, (s32)&gSharedSpriteSource,
        gSharedSpriteSource.field_4, gSharedSpriteSource.field_5, x, y);
}

void Ovl11DrawSlotQuantity(s32 orderingTable, s16 x, s16 y, s32 quantity) {
    s32 tens;

    SelectSpriteAnimation((ObjectState *)&gSharedSpriteSource, 0xA);
    x += 2;
    y += 0x16;
    SelectSpriteAnimationFrame((ObjectState *)&gSharedSpriteSource, 0xA);
    DrawSpriteFrameRects(orderingTable, (s32)&gSharedSpriteSource,
        gSharedSpriteSource.field_4, gSharedSpriteSource.field_5, x, y);

    /* Always advance over the tens position, even when it is blank. */
    x += 0xA;
    tens = quantity / 10;
    if (tens != 0) {
        SelectSpriteAnimationFrame((ObjectState *)&gSharedSpriteSource, tens & 0xFF);
        DrawSpriteFrameRects(orderingTable, (s32)&gSharedSpriteSource,
            gSharedSpriteSource.field_4, gSharedSpriteSource.field_5, x, y);
    }

    x += 0xA;
    SelectSpriteAnimationFrame((ObjectState *)&gSharedSpriteSource, (quantity % 10) & 0xFF);
    DrawSpriteFrameRects(orderingTable, (s32)&gSharedSpriteSource,
        gSharedSpriteSource.field_4, gSharedSpriteSource.field_5, x, y);
}
