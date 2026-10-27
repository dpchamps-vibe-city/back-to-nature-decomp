/* Slot descriptor installation and texture/CLUT relocation, ovl_11:800F0A58.
 * Public contract: ovl11_record_texture_relocation.h.
 * Membership/boundaries: notes/file-groupings.md. */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_record_texture_relocation.h"

s32 Ovl11RelocateRecordSlotTexture(s16 spriteId) {
    RECT rect;
    s32 row;
    u32 column;
    s16 sourceXBase;
    Ovl11A04B8Entry *record;
    Ovl11ActiveRecordView *active;
    s16 *row0;
    s16 *row1;

    row = -1;
    row0 = gSlotSpriteIds;
    row1 = row0 + 9;
    for (column = 0; column < 9; column++) {
        if (*row0 == spriteId) {
            row = 0;
            break;
        }
        if (*row1 == spriteId) {
            row = 1;
            break;
        }
        row0++;
        row1++;
    }
    if (row == -1) {
        FntPrint(gOvl11MissingSlotSpriteMessage);
        return 0;
    }
    active = (Ovl11ActiveRecordView *)D_8006C838;
    record = &active->record;
    active->record =
        ((Ovl11FarRecordTableView *)&D_8007AFF0)->records[row][column];
    sourceXBase = 0x340;
    if (record->unk18 >= 0x3F0) {
        sourceXBase = 0x3C0;
    }
    /* Descriptor +0/+2 is the texture origin, +8/+A its dimensions. */
    rect.x = sourceXBase + record->unk0;
    rect.y = (u16)record->unk2;
    rect.w = record->unk8 / 4;
    rect.h = (u16)record->unkA;
    MoveImage2(&rect, 0x2C0, 0x1C0);
    record->unk0 = 0x2C0;
    record->unk2 = 0x1C0;
    rect.w = 0x10;
    rect.h = 1;
    /* Descriptor +18/+1A supplies the CLUT origin. */
    rect.x = (u16)record->unk18;
    rect.y = record->unk1A;
    MoveImage2(&rect, 0x2C0, 0x1BF);
    record->unk18 = 0x2C0;
    record->unk1A = 0x1BF;
    *(s16 *)record->pad = 0;
    return 1;
}
