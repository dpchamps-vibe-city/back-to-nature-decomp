/*
 * Projected actor-render hierarchy: paired actors, auxiliary records and the
 * fixed scene sprites. The two leaf drawers have no other code clients.
 * They select animations from raw state codes, not inferred gameplay states.
 * Membership and conservative outer boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl19_projected_actor_render.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "sprite_source.h"
#include "ovl19_projected_sprite.h"

void Ovl19DrawActorPairSprite(s16 *actor);
void Ovl19DrawAuxRecordSprite(Ovl19Func800BAB4CArg *record);

void Ovl19DrawProjectedActorSprites(void) {
    s32 actorIndex;
    s32 auxIndex;
    s32 fixedIndex;

    for (actorIndex = 0; actorIndex < 2; actorIndex++) {
        Ovl19DrawActorPairSprite(&gOvl19ActorPairState[actorIndex * 0x24]);
    }
    for (auxIndex = 0; auxIndex < 3; auxIndex++) {
        Ovl19DrawAuxRecordSprite(&((Ovl19Func800BAB4CArg *) gOvl19AuxRecordBase)[auxIndex]);
    }
    for (fixedIndex = 0; fixedIndex < 9; fixedIndex++) {
        /* Each position has its sprite-state halfword four bytes before it. */
        Ovl19DrawProjectedSpriteState(gActiveRenderContext->field_120, (ObjectState *) &gOvl19AuxSceneSpriteSource,
                                     &gOvl19FixedSpritePositions[fixedIndex * 6], gOvl19FixedSpritePositions[fixedIndex * 6 - 2]);
    }
}

void Ovl19DrawActorPairSprite(s16 *actor) {
    SVECTOR extraPosition;
    s16 *sceneWork;
    s16 *sprite;
    s16 *otherActor;
    s16 textureMode;
    s16 state;
    s16 state9Ticks;
    s16 state10Ticks;
    s16 animation;
    s16 projectionOffsetY;
    s16 frame;
    s16 drawExtraSprite;
    u16 otherVariant;

    otherActor = gOvl19ActorPairState;
    if (actor == gOvl19ActorPairState) {
        otherActor = actor + 0x24;
    }
    projectionOffsetY = 0;
    textureMode = *actor;
    drawExtraSprite = 0;
    if (textureMode == 0) {
        SetSpriteSourceTextureOffsets((Struct_800154CC *) (actor + 0xC), 0, 0, 0x20, 1);
        animation = 0;
    } else {
        if (textureMode == 1) {
            SetSpriteSourceTextureOffsets((Struct_800154CC *) (actor + 0xC), 0, 0, *(s16 *) ((u8 *) actor + 0xA), 0);
        }
        animation = 0;
    }
    state = *(s16 *) ((u8 *) actor + 2);
    frame = -1;
    switch (state) {
    case 0:
        animation = (*(u16 *) ((u8 *) actor + 4)) + 5;
        break;
    case 1:
        animation = (*(u16 *) ((u8 *) actor + 4)) + 9;
        break;
    case 2:
        frame = actor[2];
        animation = 0;
        break;
    case 4:
        projectionOffsetY = -0xA;
        animation = (*(u16 *) ((u8 *) actor + 4)) + 0x11;
        break;
    case 6:
        if ((gOvl19StepStartTicks - 0x1E) < actor[3]) {
            projectionOffsetY = -0xA;
            animation = (*(u16 *) ((u8 *) actor + 4)) + 0x11;
        } else {
            animation = (*(u16 *) ((u8 *) actor + 4)) + 1;
        }
        break;
    case 7:
    case 8:
        if ((gOvl19StepStartTicks - 0x1E) >= actor[3]) {
            otherVariant = *(u16 *) ((u8 *) otherActor + 4);
            animation = otherVariant + 1;
            actor[2] = otherVariant;
        } else {
            animation = (*(u16 *) ((u8 *) actor + 4)) + 0xD;
        }
        drawExtraSprite = 1;
        break;
    case 9:
        state9Ticks = actor[3];
        if (state9Ticks >= (gOvl19DoubleThresholdTicks - 0x1E)) {
            projectionOffsetY = -0xA;
            animation = (*(u16 *) ((u8 *) actor + 4)) + 0x11;
        } else if (state9Ticks >= (gOvl19DoubleThresholdTicks - 0x3C)) {
            projectionOffsetY = -0xA;
            animation = (*(u16 *) ((u8 *) actor + 4)) + 0x11;
        } else {
            animation = (*(u16 *) ((u8 *) actor + 4)) + 1;
        }
        break;
    case 10:
        state10Ticks = actor[3];
        if (state10Ticks >= (gOvl19State10ThresholdTicks - 0x1E)) {
            projectionOffsetY = -0xA;
            animation = (*(u16 *) ((u8 *) actor + 4)) + 0x11;
        } else {
            if (state10Ticks >= (gOvl19State10ThresholdTicks - 0x2D)) {
                drawExtraSprite = 1;
                animation = (*(u16 *) ((u8 *) otherActor + 4)) + 0xD;
            } else {
                drawExtraSprite = 1;
                animation = (*(u16 *) ((u8 *) otherActor + 4)) + 1;
            }
            actor[2] = otherActor[2];
        }
        break;
    case 3:
    case 5:
    case 11:
        animation = (*(u16 *) ((u8 *) actor + 4)) + 1;
        break;
    }
    sprite = actor + 0xC;
    if ((*(u8 *) ((u8 *) actor + 0x1C)) != animation) {
        SelectSpriteAnimation((ObjectState *) sprite, animation & 0xFF);
    }
    if (frame != -1) {
        SelectSpriteAnimationFrame((ObjectState *) sprite, frame & 0xFF);
    }
    Ovl19DrawProjectedSprite(gActiveRenderContext->field_120, (SpriteSourceData *) sprite, (actor + 8), projectionOffsetY);
    if (drawExtraSprite == 1) {
        sceneWork = gOvl19SceneWork;
        if ((*(u8 *) ((u8 *) sceneWork + 0x1D4)) != 0x24) {
            SelectSpriteAnimation((ObjectState *) (sceneWork + 0xE8), 0x24);
        }
        extraPosition = *(SVECTOR *) (actor + 8);
        Ovl19DrawProjectedSprite(gActiveRenderContext->field_D8 + 8, (SpriteSourceData *) (sceneWork + 0xE8), (s16 *) &extraPosition, -0x18);
    }
}

void Ovl19DrawAuxRecordSprite(Ovl19Func800BAB4CArg *record) {
    ObjectState *sprite;
    s16 state;
    s32 animation;
    s32 frame;

    if (record->unk0 == 1) {
        SetSpriteSourceTextureOffsets((Struct_800154CC *) &record->sprite, 0, 0, 0, record->unk6);
    }
    animation = 0;
    frame = -1;
    state = record->unk2;
    switch (state) {
    case 0:
        animation = 0;
        break;
    case 1:
        animation = 1;
        break;
    case 3:
        animation = 3;
        break;
    case 2:
    case 4:
        animation = 2;
        break;
    }
    sprite = (ObjectState *) &record->sprite;
    if (record->sprite.field_4 != animation) {
        SelectSpriteAnimation(sprite, animation);
    }
    if (frame != -1) {
        SelectSpriteAnimationFrame(sprite, frame & 0xFF);
    }
    Ovl19DrawProjectedSprite(gActiveRenderContext->field_120, (SpriteSourceData *) sprite, record->position, 0);
}
