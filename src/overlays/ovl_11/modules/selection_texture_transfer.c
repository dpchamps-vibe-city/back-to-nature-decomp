/*
 * selection_texture_transfer — reconstructed translation unit.
 *
 * Selection-slot texture exchange between near VRAM records and the texture
 * cache, with release of no-longer-displayed slots.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_selection_texture_transfer.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"

s32 ovl_11_func_800F8404(s32 arg0);
void Ovl11TransferSelectionTexture(s32 outgoingId, s32 incomingId, s32 releaseOutgoing);

void Ovl11ReorderSelectionTextures(u16 *firstId, s32 firstPane, u16 *secondId,
                          s32 secondPane, s32 releaseOutgoing) {
    s32 secondCategory;

    secondCategory = ovl_11_func_800F8404(secondPane);
    if (ovl_11_func_800F8404(firstPane) == 2) {
        if (secondCategory != 2) {
            Ovl11TransferSelectionTexture((s32) *secondId, (s32) *firstId, releaseOutgoing);
        }
    } else if (secondCategory == 2) {
        Ovl11TransferSelectionTexture((s32) *firstId, (s32) *secondId, releaseOutgoing);
    }
}

void Ovl11ReleaseUnusedSelectionTextures(void);
s32 ovl_11_func_800D5750(s16 arg0);
void ovl_11_func_800D666C(s16 arg0);
s32 ovl_11_func_800D6730(s16 value, s16 row);
s32 ovl_11_func_800D678C(s16 arg0);
s32 ovl_11_func_800D67F4(s16 arg0);

#define imageRect (*(RECT *)gOvl11TextureTransferRect)
#define TRANSFER_SLOTS (((Ovl11FarSlotTableView *)&D_8007AFF0)->slots)
#define records (((Ovl11FarRecordTableView *)&D_8007AFF0)->records)
#define cache gOvl11TextureCache
#define cacheImage(i) (*(Ovl11A04B8Entry *)&cache[i])

void Ovl11TransferSelectionTexture(s32 outgoingId, s32 incomingId, s32 releaseOutgoing) {
    Ovl11TextureRecord backup;
    s32 oldRow;
    s16 row;
    s32 slot;
    s16 index;
    s16 special;
    s32 position;
    s32 column;
    s16 clutColumn;
    s16 page;
    s16 clutPage;
    s32 pageX;
    s32 clutY;

    outgoingId &= 0xFFFF;
    incomingId &= 0xFFFF;
    if (releaseOutgoing == 1) {
        Ovl11ReleaseUnusedSelectionTextures();
    }
    special = 0;
    position = 0;
    if (outgoingId != 0) {
        oldRow = (s16)ovl_11_func_800D5750(outgoingId) != 0;
        for (slot = 0; slot < 9; slot = (s16)(slot + 1)) {
            if (TRANSFER_SLOTS[oldRow][slot] == outgoingId) {
                break;
            }
        }
        if (slot == 9) {
            return;
        }
        if (records[oldRow][slot].unk18 >= 0x3F0) {
            special = 1;
        }
        imageRect.w = 8;
        imageRect.x = records[oldRow][slot].unk0;
        imageRect.y = records[oldRow][slot].unk2;
        imageRect.h = oldRow == 0 ? 0x20 : 0x40;
        if (special == 1) {
            imageRect.x += 0x3C0;
        } else {
            imageRect.x += 0x340;
        }
        StoreImage(&imageRect, (u_long *)(gGraphicsHeapBase + 0x4290));
        DrawSync(0);
        imageRect.w = 0x10;
        imageRect.h = 1;
        imageRect.x = records[oldRow][slot].unk18;
        imageRect.y = records[oldRow][slot].unk1A;
        StoreImage(&imageRect, (u_long *)(gGraphicsHeapBase + 0x4A90));
        DrawSync(0);
        backup.active = 1;
        backup.id = outgoingId;
        *(Ovl11A04B8Entry *)&backup = records[oldRow][slot];
        if (releaseOutgoing == 1) {
            ovl_11_func_800D666C(outgoingId);
        }
    }
    if (incomingId != 0) {
        row = (s16)ovl_11_func_800D5750(incomingId) != 0;
        for (index = 0; index < 72; index++) {
            if (cache[index].id == incomingId) {
                break;
            }
        }
        if (index == 72) {
            return;
        }
        slot = (s16)ovl_11_func_800D6730(incomingId, row);
        if (slot == -1) {
            return;
        }
        special = ovl_11_func_800D678C(row);
        TRANSFER_SLOTS[row][slot] = incomingId;
        records[row][slot] = cacheImage(index);
        imageRect.w = 8;
        imageRect.x = cache[index].x;
        imageRect.y = cache[index].y;
        imageRect.h = row == 0 ? 0x20 : 0x40;
        if (special == 1) {
            records[row][slot].unk0 = row * 8 + 0x28;
            records[row][slot].unk2 = 0xBD;
            records[row][slot].unk4 = records[row][slot].unk0;
            records[row][slot].unk6 = records[row][slot].unk2 + records[row][slot].unkA;
            MoveImage2(&imageRect, row * 8 + 0x3E8, 0xBD);
        } else {
            position = (s16)ovl_11_func_800D67F4(row);
            records[row][slot].unk0 = position * 8;
            records[row][slot].unk2 = row * 32 + 0x9B;
            records[row][slot].unk4 = records[row][slot].unk0;
            records[row][slot].unk6 = records[row][slot].unk2 + records[row][slot].unkA;
            MoveImage2(&imageRect, position * 8 + 0x340, row * 32 + 0x9B);
        }
        imageRect.w = 0x10;
        imageRect.x = cache[index].clutX;
        imageRect.h = 1;
        imageRect.y = cache[index].clutY;
        if (special == 1) {
            records[row][slot].unk18 = 0x3F0;
            records[row][slot].unk1A = row + 0xFE;
            MoveImage2(&imageRect, 0x3F0, row + 0xFE);
        } else {
            records[row][slot].unk18 = (position - (s16)(position / 4) * 4) * 16 + 0x340;
            records[row][slot].unk1A = position / 4 + (clutY = row * 2 + 0xFB);
            MoveImage2(&imageRect, (position - (s16)(position / 4) * 4) * 16 + 0x340, (s16)(position / 4) + clutY);
        }
    }
    if (outgoingId != 0) {
        for (index = 0; index < 72; index++) {
            if (cache[index].id == outgoingId) {
                return;
            }
        }
        oldRow = (s16)ovl_11_func_800D5750(outgoingId) != 0;
        for (index = 0; index < 72; index++) {
            if (cache[index].id == 0) {
                break;
            }
        }
        if (index == 72) {
            return;
        }
        column = (s16)(index / 8);
        column = (index - column * 8) * 8;
        page = index / 24;
        pageX = (page << 6) + 0x140;
        backup.x = column + pageX;
        backup.y = ((index - page * 24) / 8) << 6;
        imageRect.x = backup.x;
        imageRect.y = backup.y;
        imageRect.w = 8;
        imageRect.h = oldRow == 0 ? 0x20 : 0x40;
        LoadImage(&imageRect, (u_long *)(gGraphicsHeapBase + 0x4290));
        DrawSync(0);
        clutColumn = index / 4;
        clutPage = index / 24;
        backup.clutX = (index - clutColumn * 4) * 16 + ((clutPage << 6) + 0x140);
        backup.clutY = (index - clutPage * 24) / 4 + 0xC0;
        imageRect.x = backup.clutX;
        imageRect.y = backup.clutY;
        imageRect.w = 0x10;
        imageRect.h = 1;
        LoadImage(&imageRect, (u_long *)(gGraphicsHeapBase + 0x4A90));
        DrawSync(0);
        cache[index].id = outgoingId;
        cacheImage(index) = *(Ovl11A04B8Entry *)&backup;
    }
}

#undef imageRect
#undef TRANSFER_SLOTS
#undef records
#undef cache
#undef cacheImage

void Ovl11ReleaseUnusedSelectionTextures(void) {
    Ovl11Slots524CView *base;
    char *far_base;
    s16 sp10[36];
    s16 *src;
    s16 *q;
    s16 temp_v1;
    s16 var_a0_2;
    s32 var_a1_2;
    s32 var_v1;

    base = (Ovl11Slots524CView *) D_8006C838;
    for (var_v1 = 0; var_v1 < 0x12; var_v1++) {
        if (var_v1 < 8) {
            sp10[var_v1] = base->slots[var_v1 + 2].unk0;
        } else if (var_v1 == 8) {
            sp10[var_v1] = ((Ovl11Slots524CView *) D_8006C838)->slots[0].unk0;
        } else if (var_v1 < 17) {
            sp10[var_v1] = base->slots[var_v1 + 1].unk0;
        } else if (var_v1 == 17) {
            sp10[var_v1] = ((Ovl11Slots524CView *) D_8006C838)->slots[1].unk0;
        }
    }
    var_v1 = 0;
    far_base = (char *) D_8009AFF0;
    src = (s16 *) (far_base + 0x54A4);
    do {
        q = src + var_v1;
        var_a0_2 = *q;
        if (var_a0_2 != 0) {
            for (var_a1_2 = 0; var_a1_2 < 0x12; var_a1_2++) {
                temp_v1 = sp10[var_a1_2];
                if (temp_v1 != 0 && var_a0_2 == temp_v1) {
                    var_a0_2 = 0;
                    sp10[var_a1_2] = 0;
                    break;
                }
            }
            if (var_a0_2 != 0) {
                ovl_11_func_800D666C(var_a0_2);
            }
        }
        var_v1 += 1;
    } while (var_v1 < 0x12);
}
