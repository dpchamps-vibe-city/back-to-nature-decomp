/*
 * ID-indexed two-row sprite cache. Installation assigns texture/CLUT space,
 * copies resident pieces or uploads a disc resource, and drawing emits an
 * SPRT followed by its draw-mode packet. See ovl11_slot_sprite_cache.h.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "packet_work.h"
#include "psyq/stddef.h"
#include "cd_loader.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "sprite_source.h"
#include "sprite_upload.h"
#include "psyq/libgs.h"
#include "psyq/libapi.h"
#include "ovl11_slot_sprite_cache.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_slot_decorations.h"

void *Ovl11EmitSlotSprite(s16 slot, s16 row, s16 alternate, s32 *ot,
                         SPRT *sprite, s16 x, s16 y, s32 flags);

void Ovl11ResetSlotSpriteCache(void) {
    memset(gSlotSpriteIds, 0, 0x24);
    memset((char *)gSlotSpriteIds + 0x24, 0xFF, 0x21C);
}

void Ovl11RemoveSlotSprite(s16 id) {
    char *farBase;
    char *ids;
    s32 offset;
    s16 *slotId;
    s32 row;
    u32 remaining;
    Ovl11A04B8Entry *entry;

    if (id != 0) {
        row = Ovl11ClassifyItemFlags(id);
        remaining = 0;
        if (row == 2) {
            row = 1;
        }
        offset = row * 8;
        farBase = (char *)&D_8007AFF0;
        offset = offset + row;
        offset = offset * 2;
        ids = farBase + 0x254A4;
        slotId = (s16 *)(ids + offset);
        entry = &gSlotSpriteEntries[row][0];
        do {
            remaining += 1;
            if (*slotId == id) {
                *slotId = 0;
                memset(entry, 0xFF, 0x1E);
                return;
            }
            entry++;
            slotId += 1;
        } while (remaining < 9);
    }
}

s32 Ovl11InsertSlotSpriteId(s16 value, s16 row) {
    s16 *slotId;
    u32 slot;

    slotId = gSlotSpriteIds + row * 9;
    for (slot = 0; slot < 9; slot++) {
        if (*slotId == 0) {
            *slotId = value;
            return slot;
        }
        slotId++;
    }
    return -1;
}

s32 Ovl11CanUseSharedSlotClut(s16 row) {
    Ovl11A04B8Entry *entry;
    s32 available;
    u32 slot;

    entry = gSlotSpriteEntries[row];
    available = 1;
    slot = 0;
    do {
        if (entry->unk18 != -1) {
            if (entry->unk18 >= 0x3F0) {
                available = 0;
            }
        }
        slot++;
        entry++;
    } while (slot < 9);
    return available;
}

s32 Ovl11FindFreeSlotTextureRank(s16 row) {
    Ovl11A04B8Entry *entry;
    s32 available;
    s32 rowOffset;
    u32 rank;
    u32 slot;
    u32 u;
    Ovl11A04B8Entry *base;

    rank = 0;
    rowOffset = row * 0x10E;
    base = (Ovl11A04B8Entry *)gSlotSpriteEntries;
    entry = (Ovl11A04B8Entry *)(rowOffset + (s32)base);
    do {
        u = rank * 8;
        available = 1;
        slot = 0;
        do {
            if (u == *(s16 *)entry) {
                if (entry->unk18 < 0x3F0) {
                    available = 0;
                }
            }
            slot++;
            entry++;
        } while (slot < 9);
        if (available == 0) {
            rank++;
            entry = (Ovl11A04B8Entry *)(rowOffset + (s32)base);
            continue;
        }
        return rank;
    } while (rank < 8);
    return 0;
}

s32 Ovl11FindResidentSlotSprite(s16 id) {
    char *farBase = (char *)&D_8007AFF0;
    s16 *header = *(s16 **)(farBase + 0x25388);
    s32 setIndex;
    s16 *ids;
    s32 *offsets;
    s32 end;
    s32 index;

    if (header == 0) {
        return 0;
    }
    setIndex = header[5];
    if (setIndex == -1) {
        return 0;
    }
    ids = D_80126254[setIndex];
    offsets = gOvl11SpriteRecordSets[setIndex];
    if (ids == 0) {
        return 0;
    }
    end = ids[0] + 1;
    ids += 1;
    offsets += 1;
    index = 1;
    while (index < end) {
        if (id == ids[0]) {
            return offsets[0] + (s32)gLoadedSpriteDataBlob;
        }
        index += 1;
        ids += 1;
        offsets += 1;
    }
    return 0;
}

typedef struct {
    u16 x;
    u16 y;
    u16 width;
    u16 height;
} SlotTextureRect;

typedef struct {
    u16 textureIndex;
    u16 clutIndex;
    u8 pad[8];
} SlotPieceRef;

void Ovl11CopySlotSpritePieces(SpriteSourceData *source, u16 *lastPiece,
                              s16 count, s16 textureX, s16 textureY,
                              s16 clutX, s16 clutY) {
    s32 destinationX;
    SlotTextureRect *rects;
    s32 pieceIndex;
    SlotTextureRect *texture;
    u16 clutIndex;
    u16 x;
    SlotTextureRect *clut;
    SlotPieceRef *piece;

    piece = (SlotPieceRef *)((u8 *)lastPiece + ((count * 0xC) - 0xC));
    pieceIndex = count - 1;
    destinationX = textureX;
    for (; pieceIndex >= 0; pieceIndex--, piece = (SlotPieceRef *)((u8 *)piece - 0xC)) {
        rects = (SlotTextureRect *)source->field_1C;
        texture = rects + piece->textureIndex;
        clutIndex = piece->clutIndex;
        x = texture->x;
        gOvl11SlotSpriteCopyRect.x = x + source->field_C;
        gOvl11SlotSpriteCopyRect.y = texture->y + source->field_E;
        gOvl11SlotSpriteCopyRect.w = texture->width;
        clut = rects + clutIndex;
        gOvl11SlotSpriteCopyRect.h = texture->height;
        MoveImage((RECT *)&gOvl11SlotSpriteCopyRect, destinationX, textureY);
        gOvl11SlotSpriteCopyRect.x = clut->x + source->field_10;
        gOvl11SlotSpriteCopyRect.y = clut->y + source->field_12;
        gOvl11SlotSpriteCopyRect.w = 0x10;
        gOvl11SlotSpriteCopyRect.h = 1;
        MoveImage((RECT *)&gOvl11SlotSpriteCopyRect, clutX, clutY);
    }
}

s32 Ovl11InstallSlotSprite(s16 id) {
    s16 origin[4];
    SpriteSourceData sourceStorage;
    SpriteSourceData *source;
    s32 resident;
    s16 secondaryCount;
    s32 unusedWord;
    s16 unusedHalfword;
    s32 primaryCount;
    s32 rankGroup;
    s32 commandBase;
    s32 tableBase;
    s32 slotOrRank;
    s32 sourceAddress;
    s32 sharedClutAvailable;
    s32 unusedIndex;
    s32 row;
    u16 *command;
    u16 *secondaryPiece;
    u16 *primaryPiece;
    u16 *commands;
    Ovl11A04B8Entry *entry;
    SpriteTableRef *primaryRef;
    SpriteTableRef *secondaryRef;
    SpriteTextureRect *secondaryRect;
    SpriteTextureRect *primaryRect;

    primaryCount = 0;
    resident = 0;
    secondaryCount = 0;
    if (id == 0) {
        return 0;
    }
    row = Ovl11ClassifyItemFlags(id);
    if (row == 2) {
        row = 1;
    }
    slotOrRank = Ovl11InsertSlotSpriteId(id, (s16) row);
    if (slotOrRank == -1) {
        return 0;
    }
    sourceAddress = Ovl11FindResidentSlotSprite(id);
    if (sourceAddress == 0) {
        CdReadDiscSectorsBlocking(0, (id << 0xB) + 0x03C19000, 0x800, 0, (s32) &gMessageWindowReadBuffer);
        InitSpriteSourceData(&sourceStorage, (SpriteDataHeader *) &gMessageWindowReadBuffer);
        source = &sourceStorage;
    } else {
        resident = 1;
        InitSpriteSourceData(&sourceStorage, (SpriteDataHeader *) sourceAddress);
        source = &sourceStorage;
    }
    primaryRect = NULL;
    primaryPiece = NULL;
    secondaryRect = NULL;
    secondaryPiece = NULL;
    commands = (u16 *) source->field_28;
    commandBase = source->field_2C;
    command = (u16 *) (commandBase + commands[1]);
    entry = &gSlotSpriteEntries[row][slotOrRank];
    if ((u16) *command <= 0xFFFDU) {
        tableBase = source->field_20;
        sourceAddress = tableBase + *command * 4;
        primaryRef = (SpriteTableRef *) sourceAddress;
        primaryCount = primaryRef->count;
        tableBase = source->field_24 + primaryRef->offset;
        primaryPiece = (u16 *) (((SpriteFramePiece *) tableBase) + (primaryCount - 1));
        primaryRect = &((SpriteTextureRect *) source->field_1C)[*primaryPiece];
    }
    command = (u16 *) (commandBase + commands[3]);
    if ((u16) *command <= 0xFFFDU) {
        tableBase = source->field_20;
        sourceAddress = tableBase + *command * 4;
        secondaryRef = (SpriteTableRef *) sourceAddress;
        tableBase = source->field_24 + secondaryRef->offset;
        secondaryPiece = (u16 *) (((SpriteFramePiece *) tableBase) + ((secondaryCount = secondaryRef->count) - 1));
        secondaryRect = &((SpriteTextureRect *) source->field_1C)[*secondaryPiece];
    }
    slotOrRank = (s16) row;
    sharedClutAvailable = Ovl11CanUseSharedSlotClut((s16) slotOrRank);
    entry->unk8 = 0;
    entry->unkA = 0U;
    entry->unk10 = 0U;
    entry->unk12 = 0U;
    entry->unkC = 0;
    entry->unkE = 0U;
    entry->unk14 = 0U;
    entry->unk16 = 0U;
    if (sharedClutAvailable != 0) {
        entry->unk18 = 0x3F0;
        entry->unk1A = (s16) (row + 0xFE);
        entry->unk0 = (u16) ((row * 8) + 0x28);
        entry->unk2 = 0xBDU;
        origin[0] = 0x280;
    } else {
        slotOrRank = Ovl11FindFreeSlotTextureRank((s16) slotOrRank);
        rankGroup = slotOrRank / 4;
        entry->unk18 = (s16) (((slotOrRank - (rankGroup * 4)) * 0x10) + 0x340);
        entry->unk1A = (s16) ((row * 2) + rankGroup + 0xFB);
        entry->unk0 = (u16) (slotOrRank * 8);
        entry->unk2 = (u16) ((row << 5) + 0x9B);
        origin[0] = 0x200;
    }
    origin[1] = 0;
    if (primaryRect != NULL) {
        entry->unk8 = (s16) (primaryRect->width * 4);
        entry->unkA = (u16) primaryRect->height;
        entry->unk10 = (u16) (*(u16 *) ((u8 *) primaryPiece + 4));
        entry->unk12 = (u16) (*(u16 *) ((u8 *) primaryPiece + 6));
    }
    if (secondaryRect != NULL) {
        entry->unkC = (s16) (secondaryRect->width * 4);
        entry->unkE = (u16) secondaryRect->height;
        entry->unk14 = (u16) (*(u16 *) ((u8 *) secondaryPiece + 4));
        entry->unk16 = (u16) (*(u16 *) ((u8 *) secondaryPiece + 6));
    }
    if (secondaryPiece != NULL) {
        entry->unk4 = (u16) entry->unk0;
        if ((primaryRect->y == secondaryRect->y) && (primaryRect->x == secondaryRect->x)) {
            entry->unk6 = (u16) entry->unk2;
        } else {
            entry->unk6 = (u16) (entry->unk2 + entry->unkA);
        }
    }
    if (resident != 0) {
        if (primaryPiece != NULL) {
            Ovl11CopySlotSpritePieces(source, primaryPiece, primaryCount, origin[0] + (u16) entry->unk0 + 0x140, origin[1] + (u16) entry->unk2, entry->unk18, entry->unk1A);
        }
        if (secondaryPiece != NULL) {
            Ovl11CopySlotSpritePieces(source, secondaryPiece, (s16) secondaryCount, origin[0] + (u16) entry->unk0 + 0x140, origin[1] + (u16) entry->unk6, entry->unk18, entry->unk1A);
            return 1;
        }
        goto installed;
    }
    UploadSpriteImagesAtOffsets(D_8006CA10, origin[0] + (u16) entry->unk0, origin[1] + (u16) entry->unk2, (u16) entry->unk18 - 0x140, (u16) entry->unk1A - 0xFE);
installed:
    return 1;
}

void Ovl11DrawSlotSprite(s16 id, s16 alternate, s32 *ot, s16 x, s16 y, s32 flags) {
    s16 *first;
    s16 *second;
    s16 row;
    u32 slot;

    row = -1;
    first = gSlotSpriteIds;
    second = first + 9;
    for (slot = 0; slot < 9;) {
        if (*first == id) {
            row = 0;
            break;
        }
        if (*second == id) {
            row = 1;
            break;
        }
        slot++;
        first++;
        second++;
    }
    if (row != -1) {
        if (flags & 0xD) {
            Ovl11DrawEmptySlot((s32)(ot + 1), x, y);
        }
        if (flags & 8) {
            x += (32 - ((Ovl11FarRecordTableView *)&D_8007AFF0)->records[row][slot].unk8) / 2;
            y += (32 - ((Ovl11FarRecordTableView *)&D_8007AFF0)->records[row][slot].unkA) / 2;
        }
        if (flags & 4) {
            x += 16;
            y += 16;
            flags |= 2;
        }
        SetPacketWorkCursor(Ovl11EmitSlotSprite((s16)slot, row, alternate, ot, ReservePacketWorkBytes(0), x, y, flags));
    }
}

void *Ovl11EmitSlotSprite(s16 slot, s16 row, s16 alternate, s32 *ot,
                         SPRT *sprite, s16 x, s16 y, s32 flags) {
    Ovl11A04B8Entry *entry;
    s16 u;
    s16 v;
    s16 width;
    s16 height;
    s32 tpage;
    DR_MODE *mode;

    entry = &gSlotSpriteEntries[row][slot];
    setSprt(sprite);
    sprite->clut = getClut(entry->unk18, entry->unk1A);
    setRGB0(sprite, 0x80, 0x80, 0x80);
    setSemiTrans(sprite, 0);
    if (alternate != 0) {
        u = entry->unk4;
        v = entry->unk6;
        width = entry->unkC;
        height = entry->unkE;
        if (flags & 2) {
            s32 dx;
            s32 dy;
            dx = entry->unk14;
            dy = entry->unk16;
            x = x + dx;
            y = y + dy;
        }
    } else {
        u = entry->unk0;
        v = entry->unk2;
        width = entry->unk8;
        height = entry->unkA;
        if (flags & 2) {
            s32 dx;
            s32 dy;
            dx = entry->unk10;
            dy = entry->unk12;
            x = x + dx;
            y = y + dy;
        }
    }
    setXY0(sprite, x, y);
    setWH(sprite, width, height);
    setUV0(sprite, (u8)(u * 4), (u8)v);
    AddPrim(ot, sprite);
    /* Retain the distinct packet-cursor roles before emitting DR_MODE. */
    do {
        mode = (DR_MODE *)(sprite + 1);
        sprite = 0;
    } while (0);
    tpage = 0x340;
    if (entry->unk18 >= 0x3F0) {
        tpage = 0x3C0;
    }
    setDrawTPage(mode, 1, 1, tpage >> 6);
    AddPrim(ot, mode);
    return mode + 1;
}
