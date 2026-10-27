/* VRAM-backed sprite block bank, original ovl_11:800E351C-800E3A3C.
 * Fifty variable-size blocks are described by 51 cumulative byte offsets.
 * Upload the rectangular prefix to VRAM, retain overflow in asset scratch,
 * and recover individual blocks with a word-sum integrity check. This is a
 * snapshot cache: scratch must remain valid until the bank is replaced.
 * The RECT-compatible view preserves the original signed-halfword layout.
 * Public buffer sizes/lifetimes: ovl11_sprite_block_bank.h. */

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "asset_scratch.h"
#include "game_flags.h"
#include "psyq/libapi.h"
#include "ovl11_sprite_block_bank.h"

s32 Ovl11SumSpriteBlockWords(s32 *words, u32 byteCount);

s32 Ovl11UploadSpriteBlockBank(u8 *source, s16 xOffset, s16 yOffset) {
    Ovl11SpriteBlockRect rect;
    u8 *cursor;
    u8 *end;
    s32 blockIndex;
    s32 byteCount;
    s32 address;
    s32 *scratchBase;

    cursor = source;
    for (blockIndex = 0; blockIndex < 0x32; blockIndex++) {
        byteCount = gOvl11SpriteBlockOffsets[blockIndex + 1] - gOvl11SpriteBlockOffsets[blockIndex];
        gOvl11SpriteBlockChecksums[blockIndex] = Ovl11SumSpriteBlockWords((s32 *)cursor, byteCount);
        cursor += byteCount;
    }

    cursor = source;
    rect = gOvl11SpriteBlockBankRect;
    rect.x += xOffset;
    rect.y += yOffset;
    LoadImage((RECT *)&rect, (u_long *)cursor);
    cursor += gOvl11SpriteBlockBankRect.w * gOvl11SpriteBlockBankRect.h * 2;

    if (GetGameFlag(2) == 1) {
        DrawSync(0);
        return 0;
    }

    end = source + gOvl11SpriteBlockOffsets[50] - gOvl11SpriteBlockOffsets[0];
    byteCount = end - cursor;
    if (GetAssetScratchCapacity() >= byteCount) {
        scratchBase = &gOvl11SpriteBlockScratchBase;
        *scratchBase = GetAssetScratchEnd() - byteCount;
        address = *scratchBase;
        if (address & 3) {
            address -= address & 3;
        }
        *scratchBase = address;
        memcpy((void *)address, cursor, byteCount);
    }
    DrawSync(0);
    return 0;
}

u8 *Ovl11ReadSpriteBlock(u16 *scratch, s16 xOffset, s16 yOffset, u32 blockIndex) {
    Ovl11SpriteBlockRect rect;
    Ovl11SpriteBlockRect bounds;
    s32 row;
    s32 column;
    s32 start;
    s32 end;
    u32 byteCount;
    s32 height;
    u8 *data;
    u8 *tail;

    if (blockIndex >= 0x32U) {
        return NULL;
    }
    start = gOvl11SpriteBlockOffsets[blockIndex];
    byteCount = gOvl11SpriteBlockOffsets[blockIndex + 1] - start;
    rect = gOvl11SpriteBlockBankRect;
    rect.x = xOffset + rect.x;
    rect.y = yOffset + rect.y;
    bounds = rect;
    column = start / 2 % rect.w;
    row = start / 2 / rect.w;
    end = (s32)(start + byteCount) / 2;
    height = end / rect.w - row;
    if (end % rect.w != 0) {
        height++;
    }
    rect.h = height;
    rect.y += row;
    if (rect.y + rect.h > bounds.y + bounds.h) {
        if (rect.y < bounds.y + bounds.h) {
            rect.h -= rect.y + rect.h - (bounds.y + bounds.h);
            StoreImage((RECT *)&rect, (u_long *)scratch);
            DrawSync(0);
            data = (u8 *)scratch + column * 2;
            tail = (u8 *)scratch + rect.w * rect.h * 2;
            memcpy(tail,
                   (void *)gOvl11SpriteBlockScratchBase,
                   gOvl11SpriteBlockOffsets[blockIndex + 1] - gOvl11SpriteBlockBankRect.w * gOvl11SpriteBlockBankRect.h * 2);
        } else {
            start -= gOvl11SpriteBlockBankRect.w * gOvl11SpriteBlockBankRect.h * 2;
            data = (u8 *)gOvl11SpriteBlockScratchBase + start;
        }
    } else {
        StoreImage((RECT *)&rect, (u_long *)scratch);
        DrawSync(0);
        data = (u8 *)scratch + column * 2;
    }
    if (gOvl11SpriteBlockChecksums[blockIndex] != Ovl11SumSpriteBlockWords((s32 *)data, byteCount)) {
        return NULL;
    }
    return data;
}

s32 Ovl11GetSpriteBlockSize(s32 resourceId) {
    resourceId -= 0x15F;
    if ((u32)resourceId >= 0x32) {
        return 0;
    }
    return gOvl11SpriteBlockOffsets[resourceId + 1] - gOvl11SpriteBlockOffsets[resourceId];
}

s32 Ovl11SumSpriteBlockWords(s32 *words, u32 byteCount) {
    s32 sum;
    u32 count;
    u32 wordCount;

    if (((u32)words & 3) != 0) {
        SystemError(0x43, 0x190);
    }
    if ((byteCount & 3) != 0) {
        SystemError(0x43, 0x191);
    }
    wordCount = byteCount >> 2;
    sum = 0;
    for (count = 0; count < wordCount; count++) {
        sum += *words;
        words++;
    }
    return sum;
}
