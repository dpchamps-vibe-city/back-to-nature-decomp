/*
 * Scene sprite sources: select animation only when needed, then build packets.
 * Projected draws share scratch and advance animation after drawing. The fixed
 * centered rect source is reset on every call instead. No path presents a frame.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */
#include "ovl21_scene_sprite_draw.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "point_projection.h"

void func_800248E8(s32 otAddress, s32 x, s32 y, s32 mode);

void Ovl21DrawRecordSprite(s32 *record) {
    s16 animationBase;
    s32 animationOffset;

    animationBase = *(s16 *) ((u8 *) record + 8);
    animationOffset = *(s16 *) ((u8 *) record + 2) < 3;
    if (animationBase == 9) {
        animationOffset = animationOffset == 0;
    }
    if (animationBase == 6) {
        animationOffset = 0;
    }
    /* The comparison uses the signed selector, the setter its low byte. */
    if ((animationBase + animationOffset) !=
        (*(SpriteSourceData **) ((u8 *) record + 0x20))->field_4) {
        SelectSpriteAnimation((ObjectState *) (*(SpriteSourceData **) ((u8 *) record + 0x20)),
                              (*(u8 *) ((u8 *) record + 8) + animationOffset) & 0xFF);
    }
    Ovl21SubmitProjectedSprite(*(SpriteSourceData **) ((u8 *) record + 0x20),
                              (u16 *) ((u8 *) record + 0xA));
}

void Ovl21DrawSlotSprite(s32 *slot) {
    if ((*(s16 *) ((u8 *) slot + 4)) != (*(u8 *) ((u8 *) slot + 0x20))) {
        SelectSpriteAnimation((ObjectState *) (slot + 7), *(u8 *) ((u8 *) slot + 4));
    }
    Ovl21SubmitProjectedSprite((SpriteSourceData *) (slot + 7), (u16 *) (slot + 3));
}

void Ovl21DrawPrimarySceneSprite(s16 offsetIndex) {
    u8 *base;
    s16 position[3];
    u16 x;

    base = (u8 *) gOvl21SceneControl;
    if (base[0x688] != 0xE) {
        SelectSpriteAnimation((ObjectState *) (base + 0x684), 0xE);
    }
    x = gOvl21SceneOffsetPairs[offsetIndex].unk0;
    position[1] = -0x1F4;
    position[0] = x;
    position[2] = gOvl21SceneOffsetPairs[offsetIndex].unk2;
    Ovl21SubmitProjectedSprite((SpriteSourceData *) (base + 0x684), (u16 *) position);
}

void Ovl21DrawCenteredFrameRects(void) {
    u8 *base;

    SelectSpriteAnimation((ObjectState *)&gOvl21FrameRectSprite, 0);
    base = gOvl21FrameRectSprite - 0x6B4;
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 4, (s32)&gOvl21FrameRectSprite,
                        base[0x6B8], base[0x6B9], (s16)(HWD0 / 2), (s16)(VWD0 / 2));
}

void Ovl21SubmitProjectedSprite(SpriteSourceData *source, u16 *position) {
    s32 otAddress;

    gOvl21SpriteProjectPosition[0] = position[0] + 0x96;
    gOvl21SpriteProjectPosition[1] = position[1] - 0x64;
    gOvl21SpriteProjectPosition[2] = position[2] - 0x96;
    otAddress = gActiveRenderContext->field_120 +
        ((ProjectPointToScreen(gOvl21SpriteProjectScreen, (SVECTOR *) gOvl21SpriteProjectPosition) >> 2) * 4);
    DrawSpriteAnimationQuadsAndUpdate(otAddress, source,
        *(s16 *) ((u8 *) gOvl21SpriteProjectScreen + 0),
        *(s16 *) ((u8 *) gOvl21SpriteProjectScreen + 4));
    /* Only these four record-owned sources get the additional screen pass. */
    if ((source == ((UnkStruct800C0448 *) gOvl21SceneControl)[0].unk30) ||
        (source == ((UnkStruct800C0448 *) gOvl21SceneControl)[1].unk30) ||
        (source == ((UnkStruct800C0448 *) gOvl21SceneControl)[2].unk30) ||
        (source == ((UnkStruct800C0448 *) gOvl21SceneControl)[5].unk30)) {
        func_800248E8(otAddress,
            *(s16 *) ((u8 *) gOvl21SpriteProjectScreen + 0),
            *(s16 *) ((u8 *) gOvl21SpriteProjectScreen + 4), 0);
    }
}
