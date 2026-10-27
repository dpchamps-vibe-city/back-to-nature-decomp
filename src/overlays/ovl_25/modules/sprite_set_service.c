/* Sprite-set animation selection, sequence stepping and projected submission.
 * Projection inputs/results are shared scratch: this service is non-reentrant.
 * The scene loader must initialize the source records and their backing tables.
 * Animation selectors reset only when needed; draw-and-update helpers tick the
 * submitted source. Pause/resume edits flag 4 (animation service), not visibility.
 * Public contracts: ovl25_sprite_set_service.h. Membership/boundaries are in
 * notes/file-groupings.md; original entry order and external linkage remain. */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "sound_commands.h"
#include "packet_work.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "point_projection.h"
#include "ovl25_sprite_set_service.h"

void Ovl25DrawBiasedProjectedSprite(SpriteSourceData *source, u16 *position);
void Ovl25DrawProjectedSprite(SpriteSourceData *source, u16 *position);

void Ovl25DrawObjectRecordPair(Ovl25ObjectRecord *object) {
    SpriteSourceData *first;
    SpriteSourceData *second;
    s16 state;
    s32 firstAnimation;
    s32 secondAnimation;
    u16 *position;

    secondAnimation = -1;
    state = object->unk2;
    firstAnimation = -1;
    switch (state) {
    case 0:
        switch (object->unk0) {
        case 0:
            firstAnimation = 4;
            break;
        case 1:
        case 2:
            firstAnimation = 5;
            break;
        case 3:
        case 4:
        case 5:
            firstAnimation = 7;
            break;
        }
        break;
    case 1:
    case 6:
        firstAnimation = 0xA;
        secondAnimation = 0xB;
        break;
    case 2:
    case 3:
    case 4:
        firstAnimation = 0;
        secondAnimation = 1;
        break;
    case 5:
        firstAnimation = 2;
        secondAnimation = 3;
        break;
    case 7:
        firstAnimation = 6;
        if (object->unk0 != 0) {
            firstAnimation = 0;
            secondAnimation = 1;
        } else {
            secondAnimation = 0xC;
        }
        break;
    case 8:
        firstAnimation = 8;
        if (*(s16 *)((u8 *)object + 4) != 0) {
            firstAnimation = 0xA;
            secondAnimation = 0xB;
        } else {
            secondAnimation = 9;
        }
        break;
    }
    first = &object->record0;
    if (object->record0.field_4 != firstAnimation) {
        SelectSpriteAnimation((ObjectState *)first, firstAnimation & 0xFF);
    }
    position = object->source;
    Ovl25DrawBiasedProjectedSprite(first, position);
    if (secondAnimation != -1) {
        second = &object->record1;
        if (object->record1.field_4 != secondAnimation) {
            SelectSpriteAnimation((ObjectState *)second, secondAnimation & 0xFF);
        }
        Ovl25DrawProjectedSprite(second, position);
    }
}

void Ovl25DrawAuxiliaryRecord(Ovl25ObjectRecord *object) {
    SpriteSourceData *source;
    s16 state;
    s32 animation;
    s32 timer;

    state = object->unk2;
    animation = -1;
    switch (state) {
    case 9:
        animation = 1;
        break;
    case 10:
        animation = 0;
        break;
    case 11:
        animation = 0;
        break;
    case 12:
        timer = object->unk4 - 1;
        object->unk4 = timer;
        animation = 1;
        if ((timer << 0x10) <= 0) {
            object->unk2 = 0xB;
        }
        break;
    }
    source = &object->record0;
    if (object->record0.field_4 != animation) {
        SelectSpriteAnimation((ObjectState *)source, animation);
    }
    Ovl25DrawBiasedProjectedSprite(source, object->source);
}

void Ovl25DrawThresholdIndicator(void) {
    u16 position[4];
    ObjectState *workBase;
    s16 state;
    s8 animation;

    if (D_800BFE60 > 0x28000) {
        animation = 0;
    } else if (D_800BFE60 > 0x1E000) {
        animation = 1;
    } else if (D_800BFE60 > 0x14000) {
        animation = 2;
    } else if (D_800BFE60 > 0xA000) {
        animation = 3;
    } else {
        animation = 4;
    }
    SelectSpriteAnimation((ObjectState *)(&gOvl25PrimarySpriteBase + 3), animation);
    workBase = (ObjectState *)(&gOvl25PrimarySpriteBase + 3) - 0x93;
    position[0] = (*(u16 *)((u8 *)workBase + 0x20)) - 0x64;
    position[1] = (*(u16 *)((u8 *)workBase + 0x22)) - 0xDC;
    position[2] = (*(u16 *)((u8 *)workBase + 0x24)) + 0xDC;
    /* The fourth halfword is copied as SVECTOR padding, never projected. */
    state = *(s16 *)((u8 *)workBase + 0x12);
    if ((state != 8) && (state != 0)) {
        Ovl25DrawBiasedProjectedSprite(&gOvl25PrimarySpriteBase + 3, position);
    }
}

void Ovl25DrawPairedSceneSprites(void) {
    ObjectState *position;
    ObjectState *second;

    SelectSpriteAnimation((ObjectState *)(&gOvl25PrimarySpriteBase + 1), 0);
    second = (ObjectState *)(&gOvl25PrimarySpriteBase + 1) + 6;
    SelectSpriteAnimation(second, 1);
    position = (ObjectState *)(&gOvl25PrimarySpriteBase + 1) - 0xD;
    Ovl25DrawBiasedProjectedSprite(&gOvl25PrimarySpriteBase + 1, (u16 *)position);
    Ovl25DrawProjectedSprite((SpriteSourceData *)second, (u16 *)position);
}

void Ovl25UpdateSequenceSprites(void) {
    SVECTOR coords;
    ObjectState *second;
    ObjectState *moving;
    ObjectState *resting;
    ObjectState *advancing;
    u8 stage;

    SelectSpriteAnimation((ObjectState *)(&gOvl25SequenceSpriteBase + 2), 0);
    Ovl25DrawBiasedProjectedSprite(&gOvl25SequenceSpriteBase + 2, (u16 *)gOvl25SequenceAnchor0);
    second = (ObjectState *)(&gOvl25SequenceSpriteBase + 2) + 6;
    SelectSpriteAnimation(second, 0);
    Ovl25DrawBiasedProjectedSprite((SpriteSourceData *)second, (u16 *)gOvl25SequenceAnchor1);
    switch (gOvl25SequenceRecordStage) {
    case 0:
        moving = (ObjectState *)(&gOvl25SequenceSpriteBase + 2) + 0xC;
        SelectSpriteAnimation(moving, 0);
        Ovl25DrawBiasedProjectedSprite((SpriteSourceData *)moving, (u16 *)gOvl25SequenceAnchor2);
        if (*(&gOvl25SequenceCyclePhase + 2) >= 0x1E) {
            gOvl25SequenceRecordStage = 1;
        }
        break;
    case 1:
        advancing = (ObjectState *)(&gOvl25SequenceSpriteBase + 2) + 0xC;
        if (*(u8 *)((u8 *)(&gOvl25SequenceSpriteBase + 2) + 0x64) != 1) {
            SelectSpriteAnimation(advancing, 1);
        }
        Ovl25DrawBiasedProjectedSprite((SpriteSourceData *)advancing, (u16 *)gOvl25SequenceAnchor2);
        gOvl25SequenceAnchor2[0] += 0x12;
        if (gOvl25SequenceAnchor2[0] >= 0xDBA) {
            gOvl25SequenceRecordStage = 0xFF;
        }
        break;
    case 0xFF:
        resting = (ObjectState *)(&gOvl25SequenceSpriteBase + 2) + 0xC;
        SelectSpriteAnimation(resting, 0);
        Ovl25DrawBiasedProjectedSprite((SpriteSourceData *)resting, (u16 *)gOvl25SequenceAnchor2);
        break;
    }
    stage = *(&gOvl25SequenceCyclePhase + 2);
    switch (stage) {
    case 0:
        SelectSpriteAnimation((ObjectState *)(&gOvl25SequenceSpriteBase + 5), 0);
        Ovl25DrawBiasedProjectedSprite(&gOvl25SequenceSpriteBase + 5, (u16 *)gOvl25SequenceAnchor3);
        if (D_800BFE64 >= 0x690) {
            *(&gOvl25SequenceCyclePhase + 2) = 1;
        }
        break;
    case 0xFF:
        SelectSpriteAnimation((ObjectState *)(&gOvl25SequenceSpriteBase + 5), 0);
        Ovl25DrawBiasedProjectedSprite(&gOvl25SequenceSpriteBase + 5, (u16 *)gOvl25SequenceAnchor3);
        break;
    default:
    {
        ObjectState *base = (ObjectState *)&gOvl25SequenceSpriteBase;
        *(&gOvl25SequenceCyclePhase + 2) += 1;
        if (*(u8 *)((u8 *)base + 0xF4) != 1) {
            SelectSpriteAnimation(base + 0x1E, 1);
        }
        Ovl25DrawBiasedProjectedSprite((SpriteSourceData *)(base + 0x1E), (u16 *)gOvl25SequenceAnchor3);
        gOvl25SequenceAnchor3[0] += 0x12;
        if (gOvl25SequenceAnchor3[0] >= 0xFA6) {
            *(&gOvl25SequenceCyclePhase + 2) = 0xFF;
        }
        break;
    }
    }
    switch (gOvl25SequenceCyclePhase) {
    case 0:
        gOvl25SequenceStep = 0x3C;
        break;
    case 1:
        SelectSpriteAnimation((ObjectState *)&gOvl25SequenceSpriteBase, 0);
        Ovl25DrawBiasedProjectedSprite(&gOvl25SequenceSpriteBase, (u16 *)gOvl25SequenceAnchor4);
        gOvl25SequenceAnchor4[1] -= gOvl25SequenceStep;
        gOvl25SequenceStep -= gOvl25SequenceStep / 3;
        if (gOvl25SequenceStep < 6) {
            gOvl25SequenceCyclePhase = 2;
        }
        break;
    case 2:
        SelectSpriteAnimation((ObjectState *)&gOvl25SequenceSpriteBase, 1);
        Ovl25DrawBiasedProjectedSprite(&gOvl25SequenceSpriteBase, (u16 *)gOvl25SequenceAnchor4);
        gOvl25SequenceAnchor4[1] += gOvl25SequenceStep;
        gOvl25SequenceStep += (gOvl25SequenceStep * 6) / 5;
        if (gOvl25SequenceAnchor4[1] >= 0) {
            gOvl25SequenceCyclePhase = 0;
            gOvl25SequenceEffectActive = 1;
            PlaySoundEffect(0x4D);
            SelectSpriteAnimation((ObjectState *)D_800C021C, 0x17);
        }
        break;
    }
    if (gOvl25SequenceEffectActive != 0) {
        /* This bias-adjusted local copy is not the submitted anchor. */
        coords = *(SVECTOR *)gOvl25SequenceAnchor4;
        coords.vx += 0x96;
        coords.vy += 0x64;
        coords.vz += 0x96;
        Ovl25DrawBiasedProjectedSprite((SpriteSourceData *)D_800C021C, (u16 *)gOvl25SequenceAnchor4);
        if (((SpriteSourceData *)D_800C021C)->field_2 & 0x100) {
            gOvl25SequenceEffectActive = 0;
        }
    }
}

void Ovl25PauseSpriteSets(void) {
    u16 *second;
    u16 *first;
    u16 *primaryBase;
    s32 index;
    u16 *auxiliaryBase;

    AddSpriteSourceFlags((u16 *)&gOvl25PrimarySpriteBase, 4);
    AddSpriteSourceFlags((u16 *)&gOvl25PrimarySpriteBase + 0x48, 4);
    AddSpriteSourceFlags((u16 *)&gOvl25PrimarySpriteBase + 0x18, 4);
    AddSpriteSourceFlags((u16 *)&gOvl25PrimarySpriteBase + 0x30, 4);
    primaryBase = (u16 *)&gOvl25PrimarySpriteBase;
    second = primaryBase - 0x1D8;
    first = primaryBase - 0x1F0;
    for (index = 5; index >= 0; index--) {
        AddSpriteSourceFlags(first, 4);
        AddSpriteSourceFlags(second, 4);
        second += 0x3C;
        first += 0x3C;
    }
    auxiliaryBase = (u16 *)&gOvl25AuxiliarySpriteBase;
    first = auxiliaryBase + 0x18;
    second = auxiliaryBase;
    for (index = 1; index >= 0; index--) {
        AddSpriteSourceFlags(second, 4);
        AddSpriteSourceFlags(first, 4);
        first += 0x3C;
        second += 0x3C;
    }
    AddSpriteSourceFlags((u16 *)&gOvl25SequenceSpriteBase, 4);
    AddSpriteSourceFlags((u16 *)&gOvl25SequenceSpriteBase + 0x18, 4);
    AddSpriteSourceFlags((u16 *)&gOvl25SequenceSpriteBase + 0x30, 4);
    AddSpriteSourceFlags((u16 *)&gOvl25SequenceSpriteBase + 0x48, 4);
    AddSpriteSourceFlags((u16 *)&gOvl25SequenceSpriteBase + 0x60, 4);
    AddSpriteSourceFlags((u16 *)&gOvl25SequenceSpriteBase + 0x78, 4);
}

void Ovl25ResumeSpriteSets(void) {
    u16 *second;
    u16 *first;
    u16 *primaryBase;
    s32 index;
    u16 *auxiliaryBase;

    ClearSpriteSourceFlags((u16 *)&gOvl25PrimarySpriteBase, 4);
    ClearSpriteSourceFlags((u16 *)&gOvl25PrimarySpriteBase + 0x48, 4);
    ClearSpriteSourceFlags((u16 *)&gOvl25PrimarySpriteBase + 0x18, 4);
    ClearSpriteSourceFlags((u16 *)&gOvl25PrimarySpriteBase + 0x30, 4);
    primaryBase = (u16 *)&gOvl25PrimarySpriteBase;
    second = primaryBase - 0x1D8;
    first = primaryBase - 0x1F0;
    for (index = 5; index >= 0; index--) {
        ClearSpriteSourceFlags(first, 4);
        ClearSpriteSourceFlags(second, 4);
        second += 0x3C;
        first += 0x3C;
    }
    auxiliaryBase = (u16 *)&gOvl25AuxiliarySpriteBase;
    first = auxiliaryBase + 0x18;
    second = auxiliaryBase;
    for (index = 1; index >= 0; index--) {
        ClearSpriteSourceFlags(second, 4);
        ClearSpriteSourceFlags(first, 4);
        first += 0x3C;
        second += 0x3C;
    }
    ClearSpriteSourceFlags((u16 *)&gOvl25SequenceSpriteBase, 4);
    ClearSpriteSourceFlags((u16 *)&gOvl25SequenceSpriteBase + 0x18, 4);
    ClearSpriteSourceFlags((u16 *)&gOvl25SequenceSpriteBase + 0x30, 4);
    ClearSpriteSourceFlags((u16 *)&gOvl25SequenceSpriteBase + 0x48, 4);
    ClearSpriteSourceFlags((u16 *)&gOvl25SequenceSpriteBase + 0x60, 4);
    ClearSpriteSourceFlags((u16 *)&gOvl25SequenceSpriteBase + 0x78, 4);
}

void Ovl25DrawPrimarySprite(void) {
    ObjectState *anchor;

    SelectSpriteAnimation((ObjectState *)&gOvl25PrimarySpriteBase, 0);
    anchor = (ObjectState *)&gOvl25PrimarySpriteBase - 0x81;
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 4,
        (s32)&gOvl25PrimarySpriteBase,
        (s32)(*(u8 *)((u8 *)anchor + 0x40C)),
        (s32)(*(u8 *)((u8 *)anchor + 0x40D)),
        (s16)((s32)((HWD0 + ((u32)HWD0 >> 0x1F)) << 0xF) >> 0x10),
        (s16)((s32)((VWD0 + ((u32)VWD0 >> 0x1F)) << 0xF) >> 0x10));
}

void func_800248E8(s32 orderingTable, s32 x, s32 y, s32 variant);

void Ovl25DrawBiasedProjectedSprite(SpriteSourceData *source, u16 *position) {
    s32 orderingTable;

    gOvl25BiasedProjectionInput[0] = position[0] - 0x96;
    gOvl25BiasedProjectionInput[1] = position[1] - 0x64;
    gOvl25BiasedProjectionInput[2] = position[2] - 0x96;
    gOvl25BiasedProjectionInput[3] = position[3];
    orderingTable = gActiveRenderContext->field_120 +
        ((ProjectPointToScreen(gOvl25BiasedProjectionResult,
          (SVECTOR *)gOvl25BiasedProjectionInput) >> 2) * 4);
    DrawSpriteAnimationQuadsAndUpdate(orderingTable, source,
        *(s16 *)((u8 *)gOvl25BiasedProjectionResult + 0),
        *(s16 *)((u8 *)gOvl25BiasedProjectionResult + 4));
    /* Identity, not animation number, selects the additional marker sprite. */
    if (D_800BFE44_view.selector == 0) {
        if ((source == ((s16 *)&D_800BFE44_view + 0x14)) || (source == ((s16 *)&D_800BFE44_view + 0x50)) || (source == ((s16 *)&D_800BFE44_view + 0x8C)) || (source == ((s16 *)&D_800BFE44_view + 0xC8)) || (source == ((s16 *)&D_800BFE44_view + 0x104)) || (source == ((s16 *)&D_800BFE44_view + 0x140)) || (source == ((s16 *)&D_800BFE44_view + 0x17C)) || (source == ((s16 *)&D_800BFE44_view + 0x1B8))) {
            func_800248E8(orderingTable, *(s16 *)((u8 *)gOvl25BiasedProjectionResult + 0), *(s16 *)((u8 *)gOvl25BiasedProjectionResult + 4), 0);
        }
    } else if ((source == ((s16 *)&D_800BFE44_view + 0x17C)) || (source == ((s16 *)&D_800BFE44_view + 0x1B8))) {
        func_800248E8(orderingTable, *(s16 *)((u8 *)gOvl25BiasedProjectionResult + 0), *(s16 *)((u8 *)gOvl25BiasedProjectionResult + 4), 0);
    }
    if ((source == &gOvl25SequenceSpriteBase) || (source == ((ObjectState *)&gOvl25SequenceSpriteBase + 6)) || (source == ((ObjectState *)&gOvl25SequenceSpriteBase + 0xC)) || (source == ((ObjectState *)&gOvl25SequenceSpriteBase + 0x12)) || (source == ((ObjectState *)&gOvl25SequenceSpriteBase + 0x18)) || (source == ((ObjectState *)&gOvl25SequenceSpriteBase + 0x1E))) {
        func_800248E8(orderingTable, *(s16 *)((u8 *)gOvl25BiasedProjectionResult + 0), *(s16 *)((u8 *)gOvl25BiasedProjectionResult + 4), 0);
    }
}

void Ovl25DrawProjectedSprite(SpriteSourceData *source, u16 *position) {
    s32 depth;
    s32 offset;
    s32 orderingTableBase;

    gOvl25ProjectionInput[0] = position[0];
    gOvl25ProjectionInput[1] = position[1];
    gOvl25ProjectionInput[2] = position[2];
    gOvl25ProjectionInput[3] = position[3];
    depth = ProjectPointToScreen(gOvl25ProjectionResult, (SVECTOR *)gOvl25ProjectionInput);
    offset = depth + 0x400;
    orderingTableBase = gActiveRenderContext->field_120;
    DrawSpriteAnimationQuadsAndUpdate(orderingTableBase + (offset >> 2) * 4,
        source, ((s16 *)gOvl25ProjectionResult)[0], ((s16 *)gOvl25ProjectionResult)[2]);
}
