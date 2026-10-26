/*
 * Complementary record-sprite drawers and their shared threshold indicator.
 * Coordinates are 12-bit fixed point before projection; both paths preserve
 * signed truncation toward zero. See ovl17_record_sprite_draw.h for side effects.
 * Membership and conservative outer boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl17_record_sprite_draw.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void func_800248E8(s32 orderingTable, s32 x, s32 y, s32 frame);
void Ovl17SelectRecordIndicatorAnimation(void);
void ovl_17_func_800B9158(s32 position, s16 y);

void Ovl17DrawRecordSpriteQuads(Recon_ovl_17_func_800BAD9C_A0View *record, s32 viewOriginX) {
    u8 *sprite;
    Recon_ovl_17_func_800BAD9C_AnimRef *display;
    s16 y;
    s16 x;
    s32 orderingTable;
    s32 delta;
    s32 pixelDelta;
    s32 hasIndicator;

    delta = record->field_10 - viewOriginX;
    pixelDelta = delta >> 0xC;
    if (delta < 0) {
        pixelDelta = (s32) (delta + 0xFFF) >> 0xC;
    }
    x = 0xA0 - pixelDelta;
    display = record->field_14;
    orderingTable = gActiveRenderContext->field_D8 + (display->field_4 * 4);
    hasIndicator = record->field_0 & 1;
    sprite = (u8 *) record + 0x20;
    y = display->field_0;
    if (!hasIndicator) {
        SetSpriteSourceTextureOffsets((Struct_800154CC *) sprite, 0, 0, 0, (s16) (s32) display->field_2);
    }
    DrawSpriteAnimationQuadsAndUpdate(orderingTable, (SpriteSourceData *) sprite, x, y);
    func_800248E8(orderingTable, x, y, 1);
    if (record->field_0 & 1) {
        Ovl17SelectRecordIndicatorAnimation();
        DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x10, (SpriteSourceData *) gOvl17ThresholdIndicatorSprite, (s16) (x - 0x38), (s16) (y - 0x38));
    }
    /* Keep the unsigned quotient test: this is not a signed remainder. */
    if (gOvl17RecordDrawTick == ((((gOvl17RecordDrawTick + 1) / 10) * 0xA) - 1)) {
        ovl_17_func_800B9158(record->field_10, y);
    }
}

void Ovl17DrawRecordSpriteRects(Recon_ovl_17_func_800BAD9C_A0View *record, s32 viewOriginX) {
    Struct_800154CC *sprite;
    Recon_ovl_17_func_800BAD9C_AnimRef *display;
    s16 y;
    s16 x;
    s32 orderingTable;
    s32 delta = record->field_10 - viewOriginX;
    s32 pixelDelta;
    s32 hasIndicator;
    u8 *work;
    u8 *indicator;

    pixelDelta = delta >> 0xC;
    if (delta < 0) {
        pixelDelta = (delta + 0xFFF) >> 0xC;
    }
    x = (s16)(0xA0 - pixelDelta);
    display = record->field_14;
    orderingTable = gActiveRenderContext->field_D8 + (display->field_4 * 4);
    hasIndicator = record->field_0 & 1;
    sprite = (Struct_800154CC *)((u8 *)record + 0x20);
    y = display->field_0;
    if (!hasIndicator) {
        SetSpriteSourceTextureOffsets(sprite, 0, 0, 0, display->field_2);
    }
    DrawSpriteFrameRects(orderingTable, (s32)sprite, (s32)record->field_24, (s32)record->field_25,
                        x, y);
    func_800248E8(orderingTable, x, y, 1);
    if (record->field_0 & 1) {
        Ovl17SelectRecordIndicatorAnimation();
        indicator = gOvl17ThresholdIndicatorSprite;
        work = indicator - 0x28C;
        DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x10, (s32)indicator,
                             (s32)work[0x290], (s32)work[0x291],
                             (s16)(x - 0x38), (s16)(y - 0x38));
    }
}

void Ovl17SelectRecordIndicatorAnimation(void) {
    u8 *work;
    u8 *spriteWork;
    s32 animation;
    s16 value;

    work = gOvl17RecordWorkBase;
    value = *(s16 *)(work + 0xCA);
    if (value >= 0xAA) {
        animation = 0;
    } else if (value >= 3) {
        animation = 1;
    } else {
        animation = 2;
    }
    spriteWork = gOvl17RecordWorkBase;
    if (*(u8 *)(spriteWork + 0x290) != animation) {
        SelectSpriteAnimation((ObjectState *)(spriteWork + 0x28C), (s8)animation);
    }
}
