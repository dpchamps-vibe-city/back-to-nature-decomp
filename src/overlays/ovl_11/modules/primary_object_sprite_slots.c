/*
 * Primary-object sprite-slot banks (ovl_11:800CDE3C-800CE210).
 * Initializers populate the position/source state consumed by both draw
 * walkers and their common projection helper. The intervening grid-offset
 * utility shares the object's position convention, but not bank ownership.
 * Contracts: ovl11_primary_object_sprite_slots.h. Membership and boundaries:
 * notes/file-groupings.md. All resident and overlay storage remains external.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_primary_object_sprite_slots.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_projected_record_sprites.h"

void Ovl11DrawObjectSpriteSlot(Ovl11SpritePositionView *position, u8 *object,
                              s32 mode, u32 index);
void func_80016054(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s16 arg5,
                  s16 arg6, s32 arg7, s32 arg8, u16 arg9);

s32 Ovl11InitObjectSpriteSlots(u8 *object) {
    s32 headerOffset;
    u32 index;
    u8 *positionTail;
    u8 *sourceCursor;
    u8 *bank;

    index = 0;
    positionTail = object + 0x128;
    do {
        *(s32 *)(positionTail - 8) = *(s32 *)(object + 0x100);
        *(s32 *)(positionTail - 4) = *(s32 *)(object + 0x104);
        index += 1;
        *(s32 *)positionTail = *(s32 *)(object + 0x108);
        positionTail += 0x10;
    } while (index < 0xAU);
    index = 0;
    bank = (u8 *)&gFixedIdSpriteBankBase;
    *(s16 *)(object + 0x32) = 0;
    sourceCursor = object + 0x2D4;
    headerOffset = gOvl11SpriteOffsetRegistry.fixedIdOffsets[1];
    do {
        InitSpriteSourceData((SpriteSourceData *)sourceCursor,
                             (SpriteDataHeader *)(headerOffset + (s32)bank));
        index += 1;
        sourceCursor += 0x34;
    } while (index < 5U);
    return 0;
}

u32 Ovl11DrawEmbeddedSpriteSlots(u8 *object) {
    u32 index;
    u8 *position;

    index = 0;
    position = object + 0x120;
    do {
        Ovl11DrawObjectSpriteSlot((Ovl11SpritePositionView *)position, object, 0, index);
        index += 1;
        position += 0x10;
    } while (index < 5U);
    return 0U;
}

s32 Ovl11InitSharedSpriteSlots(u8 *object) {
    u8 *positionBase;
    s16 *frame;
    u32 index;
    s32 inactiveAnimation;

    index = 0;
    inactiveAnimation = 5;
    positionBase = object;
    frame = (s16 *)(positionBase + 0x3D6);
    do {
        frame[-1] = (s16)inactiveAnimation;
        frame[0] = (s16)(index & 1);
        *(s32 *)(positionBase + 0x1C0) = *(s32 *)(object + 0x100);
        *(s32 *)(positionBase + 0x1C4) = *(s32 *)(object + 0x104);
        frame += 2;
        *(s32 *)(positionBase + 0x1C8) = *(s32 *)(object + 0x108);
        index += 1;
        *(s32 *)(positionBase + 0x1CC) = *(s32 *)(object + 0x10C);
        positionBase += 0x10;
    } while (index < 10);
    return 0;
}

u32 Ovl11DrawSharedSpriteSlots(u8 *object) {
    u32 index;
    u32 inactiveAnimation;
    u8 *animation;
    u8 *position;

    index = 0;
    inactiveAnimation = 5;
    position = object + 0x1C0;
    animation = object + 0x3D4;
    do {
        if (*(u16 *)animation != inactiveAnimation) {
            Ovl11DrawObjectSpriteSlot((Ovl11SpritePositionView *)position, object, 1, index);
        }
        position += 0x10;
        index += 1;
        animation += 4;
    } while (index < 0xAU);
    return 0U;
}

void Ovl11ComputeGridOffsetPosition(Ovl11CE034Object *object,
                                   Ovl11CE034Position *position, s32 mode) {
    u16 *cellOffset;
    s32 x;
    s32 z;

    cellOffset = (u16 *)(gOvl11AreaGridCellOffsets + object->unk38 * 4);
    if (mode == 2) {
        cellOffset = (u16 *)(gOvl11AreaGridCellOffsets + object->unk38 * 4 + 0x10);
    }
    x = object->unk100;
    position->unk0 = x;
    position->unk4 = object->unk104;
    z = object->unk108;
    position->unk8 = z;
    if (mode == 4) {
        position->unk0 = x + (s16)cellOffset[0] / 2;
        position->unk8 = z + (s16)cellOffset[1] / 2;
        return;
    }
    position->unk0 = x + (s16)cellOffset[0];
    position->unk8 = z + (s16)cellOffset[1];
}

void Ovl11DrawObjectSpriteSlot(Ovl11SpritePositionView *position, u8 *object,
                              s32 mode, u32 index) {
    SpriteSourceData *source;
    s32 state;
    s32 animation;
    s32 frame;
    u32 work;
    u16 input[4];
    s32 projected[4];

    if (mode != 0) {
        source = &gOvl11SharedSlotSpriteSource;
        state = index * 4;
        /* Retain the two witnessed halfword/word-stride access forms. */
        animation = *(s16 *)(object - (state >> 1) * -2 + 0x3D4);
        frame = *(s16 *)(object - (state >> 2) * -4 + 0x3D6);
    } else {
        /* All five embedded-mode positions use the FIRST source, regardless
         * of index. Animation can therefore advance once per drawn position. */
        source = (SpriteSourceData *)(object + 0x2D4);
        animation = source->field_4;
        state = source->field_5;
        frame = (s16)state;
    }
    work = (u32)input;
    input[0] = position->x + 0x96;
    input[1] = position->y - 0x64;
    input[2] = position->z - 0x96;
    if (Ovl11ProjectRecordSprite((u16 *)work, projected) != 0) {
        func_80016054(gActiveRenderContext->field_120 + ((projected[2] >> 2) * 4),
                      gActiveRenderContext->field_118, (s32)source,
                      (s32)(animation & 0xFF), (u8)(frame & 0xFF),
                      (s16)projected[0], (s16)projected[1], source->field_8,
                      0, 0x40);
    }
    if (mode == 0) {
        work = 0x08000000;
        if (!(D_8006C844 & work)) {
            UpdateSpriteAnimation(source);
        }
    }
}
