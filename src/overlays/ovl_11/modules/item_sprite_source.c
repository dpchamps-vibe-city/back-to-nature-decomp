/*
 * item_sprite_source — reconstructed translation unit.
 *
 * Install item sprite sources through four private seasonal maps and the
 * set-4/set-7 maps. Kind 1 uses the season, kinds 4/5 use set 7 and kind 6
 * uses set 4. A loaded header suppresses repeated installation; unsupported
 * IDs clear both the item and header fields. The embedded SpriteSourceData
 * header at +C8+14 explains the former anonymous +DC latch.
 * Public contract: ovl11_item_sprite_source.h. The switch maps and biased
 * signed-halfword dispatch retain their exact original mapping behavior.
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

#include "ovl11_item_sprite_source.h"

s32 Ovl11MapSet7ItemSprite(s16 itemId);
s32 Ovl11MapSet4ItemSprite(s16 itemId);

void Ovl11InstallItemSpriteSource(Ovl11ItemSpriteObject *object) {
    Ovl11SeasonItemSpriteMaps maps;
    SpriteDataHeader *header;
    s16 sourceKind;
    s32 spriteIndex;
    s32 setIndex;
    u16 itemId;
    char *farWork;

    maps = gOvl11SeasonItemSpriteMaps;
    itemId = object->itemId;
    if (itemId == 0) {
        object->spriteHeader = 0;
        return;
    }
    if (object->spriteHeader != 0) {
        return;
    }
    sourceKind = object->sourceKind;
    farWork = (char *)&D_8007AFF0;
    if (sourceKind == *(s16 *)(farWork + 0x25476)) {
        switch (sourceKind) {
        case 1:
            setIndex = maps.entries[gCalendarSeason].setIndex;
            spriteIndex = maps.entries[gCalendarSeason].mapItemId((s16)itemId);
            break;
        case 4:
        case 5:
            spriteIndex = Ovl11MapSet7ItemSprite((s16)itemId);
            setIndex = 7;
            break;
        case 6:
            spriteIndex = Ovl11MapSet4ItemSprite((s16)itemId);
            setIndex = 4;
            break;
        default:
            spriteIndex = -1;
            setIndex = 0;
            break;
        }
        if (spriteIndex == -1) {
            object->itemId = 0;
            object->spriteHeader = 0;
            return;
        }
        /* Word zero is the set count; following words are blob offsets. */
        header = (SpriteDataHeader *)(((s32 *)gOvl11SpriteRecordSets[setIndex])[spriteIndex + 1] + (s32)gLoadedSpriteDataBlob);
        InitSpriteSourceData((SpriteSourceData *)((u8 *)object + 0xC8), header);
    }
}

s32 Ovl11MapSeason0ItemSprite(s16 itemId) {
    switch (itemId) {
    case 0x41:
        return 7;
    case 0x42:
        return 8;
    case 0x43:
        return 9;
    case 0x44:
        return 0xA;
    case 0x45:
        return 0xB;
    case 0x51:
        return 0xC;
    case 0x55:
        return 0xD;
    case 0xE8:
        return 0x17;
    case 0x8B:
        return 0x18;
    case 0x8C:
        return 0x19;
    case 0x8D:
        return 0x1A;
    case 0xE3:
        return 0x1B;
    case 0x91:
        return 0x1C;
    case 0x92:
        return 0x1D;
    case 0x93:
        return 0x1E;
    case 0xE7:
        return 0x1F;
    case 0x122:
    case 0x123:
    case 0x124:
        return 0x16;
    }
    return -1;
}

s32 Ovl11MapSeason1ItemSprite(s16 itemId) {
    switch (itemId) {
    case 0x46:
        return 0xC;
    case 0x47:
        return 0xD;
    case 0x48:
        return 0xE;
    case 0x49:
        return 0xF;
    case 0x4A:
        return 0x10;
    case 0x52:
        return 0x11;
    case 0x56:
        return 0x12;
    case 0xE8:
        return 0x1C;
    case 0x8B:
        return 0x1D;
    case 0x8C:
        return 0x1E;
    case 0x8D:
        return 0x1F;
    case 0xE3:
        return 0x20;
    case 0x91:
        return 0x21;
    case 0x92:
        return 0x22;
    case 0x93:
        return 0x23;
    case 0xE7:
        return 0x24;
    case 0x122:
    case 0x123:
    case 0x124:
        return 0x1B;
    }
    return -1;
}

s32 Ovl11MapSeason2ItemSprite(s16 itemId) {
    switch (itemId) {
    case 0x4B:
        return 0xC;
    case 0x4C:
        return 0xD;
    case 0x4D:
        return 0xE;
    case 0x4E:
        return 0xF;
    case 0x4F:
        return 0x10;
    case 0x53:
        return 0x11;
    case 0x54:
        return 0x19;
    case 0xE8:
        return 0x1C;
    case 0x8B:
        return 0x1D;
    case 0x8C:
        return 0x1E;
    case 0x8D:
        return 0x1F;
    case 0xE3:
        return 0x20;
    case 0x91:
        return 0x21;
    case 0x92:
        return 0x22;
    case 0x93:
        return 0x23;
    case 0xE7:
        return 0x24;
    case 0x122:
    case 0x123:
    case 0x124:
        return 0x1B;
    }
    return -1;
}

s32 Ovl11MapSeason3ItemSprite(s16 itemId) {
    switch (itemId) {
    case 0x122:
    case 0x123:
    case 0x124:
        return 0xC;
    case 0xE8:
        return 0xD;
    case 0x8B:
        return 0xE;
    case 0x8C:
        return 0xF;
    case 0x8D:
        return 0x10;
    case 0xE3:
        return 0x11;
    case 0x91:
        return 0x12;
    case 0x92:
        return 0x13;
    case 0x93:
        return 0x14;
    case 0xE7:
        return 0x15;
    }
    return -1;
}

s32 Ovl11MapSet4ItemSprite(s16 itemId) {
    s32 biasedId;

    biasedId = (s32)((itemId << 0x10) + 0xFFBF0000) >> 0x10;
    switch (biasedId) {
    case 0x0:
        return 4;
    case 0x1:
        return 5;
    case 0x2:
        return 6;
    case 0x3:
        return 7;
    case 0x4:
        return 8;
    case 0x5:
        return 9;
    case 0x6:
        return 0xA;
    case 0x7:
        return 0xB;
    case 0x8:
        return 0xC;
    case 0x9:
        return 0xD;
    case 0xA:
        return 0xE;
    case 0xB:
        return 0xF;
    case 0xC:
        return 0x10;
    case 0xD:
        return 0x11;
    case 0xE:
        return 0x12;
    case 0x10:
        return 0x14;
    case 0x11:
        return 0x15;
    case 0x12:
        return 0x16;
    case 0x13:
        return 0x1B;
    case 0x14:
        return 0x17;
    case 0x15:
        return 0x18;
    case 0xE1:
    case 0xE2:
    case 0xE3:
        return 0x1D;
    case 0xA7:
        return 0x1E;
    default:
        return -1;
    }
}

s32 Ovl11MapSet7ItemSprite(s16 itemId) {
    switch (itemId) {
    case 0x68:
        return 0;
    case 0x6B:
        return 1;
    case 0x8B:
        return 8;
    case 0x8C:
        return 9;
    case 0x8D:
        return 0xA;
    case 0xE3:
        return 0xB;
    case 0x91:
        return 0xC;
    case 0x92:
        return 0xD;
    case 0x93:
        return 0xE;
    case 0xE7:
        return 0xF;
    case 0xE8:
        return 6;
    case 0x122:
    case 0x123:
    case 0x124:
        return 5;
    default:
        return -1;
    }
}
