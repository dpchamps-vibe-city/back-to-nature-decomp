/*
 * Fixed pool of projected sprite animations, ovl_11:801212C4..80121780.
 * A NULL sprite marks a free entry. The pool does not own source-bank art or
 * render packets; it borrows both from their existing global services.
 * Public units/constraints are in ovl11_sprite_effect_pool.h; membership and
 * original-address provenance are recorded in notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_sprite_effect_pool.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "ovl11_projected_record_sprites.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_cell_grid_lookup.h"

/* Helpers retain external linkage. */
void Ovl11DrawAndStepSpriteEffect(Ovl11SpriteEffect *effect);
void Ovl11ReleaseSpriteEffect(Ovl11SpriteEffect *effect);
void Ovl11InitSpriteEffect(Ovl11SpriteEffect *effect, VECTOR position, u16 id);

void Ovl11UpdateSpriteEffects(void) {
    Ovl11SpriteEffect *effect;
    s32 remaining;

    if (!(gSharedWorkFlags & 0x400)) {
        effect = (Ovl11SpriteEffect *)gOvl11SpriteEffects;
        remaining = 0x18;
        do {
            Ovl11DrawAndStepSpriteEffect(effect);
            remaining -= 1;
            effect += 1;
        } while (remaining >= 0);
    }
}

void Ovl11ClearSpriteEffects(void) {
    u8 *effect;
    s32 remaining;

    effect = gOvl11SpriteEffects;
    remaining = 0x18;
    do {
        Ovl11ReleaseSpriteEffect((Ovl11SpriteEffect *)effect);
        remaining -= 1;
        effect += 0x18;
    } while (remaining >= 0);
}

/* VECTOR's by-value ABI preserves all four incoming words, even though the
 * initializer consumes only its three coordinates. Do not split it into
 * scalar parameters: callers already pass the same 16-byte aggregate. */
s32 Ovl11SpawnSpriteEffect(s32 id, VECTOR position) {
    Ovl11SpriteEffect *effect;
    s32 i;

    effect = (Ovl11SpriteEffect *)gOvl11SpriteEffects;
    i = 0;
    id &= 0xFFFF;
    for (; i < 0x19; i++) {
        if (effect->sprite == 0) {
            Ovl11InitSpriteEffect(effect, position, id);
            return 1;
        }
        effect++;
    }
    return 0;
}

void Ovl11DrawAndStepSpriteEffect(Ovl11SpriteEffect *effect) {
    u16 position[3];
    s32 screen[3];
    s16 frames;
    s16 frame;
    s32 playing;
    u16 tick;

    if (effect->sprite != 0) {
        frames = GetSpriteAnimationFrameCount(effect->sprite, effect->animation);
        tick = effect->tick;
        frame = (s16)tick / 3;
        if (D_8006C844 & 0x08000000) {
            playing = frame < frames;
        } else {
            effect->tick = tick + 1;
            playing = frame < frames;
        }
        if (playing != 0) {
            position[0] = (u16)effect->x + 0x96;
            position[1] = (u16)effect->y - 0x64;
            position[2] = (u16)effect->z - 0x96;
            if (Ovl11ProjectRecordSprite(position, screen) != 0) {
                DrawSpriteFrameRects(gActiveRenderContext->field_120 + ((screen[2] >> 2) * 4), (s32)effect->sprite,
                                     (u8)effect->animation, frame & 0xFF, screen[0], screen[1]);
            }
        } else {
            Ovl11ReleaseSpriteEffect(effect);
        }
    }
}

void Ovl11ReleaseSpriteEffect(Ovl11SpriteEffect *effect) {
    effect->sprite = 0;
}

#define EFFECT_SEASON (((struct struct_8006C838_button *)D_8006C838)->field_44BA)
#define EFFECT_AREA (((struct Ovl11D2764StateView *)&D_8007AFF0)->field_25476)

void Ovl11InitSpriteEffect(Ovl11SpriteEffect *effect, VECTOR position, u16 id) {
    s32 animation;
    s32 itemSeason;
    s32 sourceBase;
    s32 seasonalAnimation;
    s32 sourceIndex;

    seasonalAnimation = 0;
    sourceBase = 0;
    if (EFFECT_SEASON == 3) {
        seasonalAnimation = (EFFECT_AREA != 1) * 2;
    }
    if (EFFECT_AREA != 1) {
        sourceBase = -1;
    }
    if (EFFECT_AREA == 6) {
        sourceBase = 0x16;
    }
    switch (id) {
    case 0x3A:
        sourceIndex = 0;
        animation = seasonalAnimation + 1;
        break;
    case 0x3B:
    case 0x16C:
    case 0x16D:
    case 0x16E:
        sourceIndex = 1;
        animation = seasonalAnimation + 1;
        break;
    case 0x3C:
    case 0x16F:
    case 0x170:
    case 0x171:
        sourceIndex = 2;
        animation = seasonalAnimation + 1;
        break;
    case 0x3D:
    case 0x172:
    case 0x173:
    case 0x174:
        sourceIndex = 3;
        animation = seasonalAnimation + 1;
        break;
    case 0x3E:
        sourceIndex = 4;
        animation = seasonalAnimation + 1;
        break;
    case 0x3F:
    case 0x175:
        sourceIndex = 5;
        animation = 1;
        break;
    case 0x36:
        animation = 1;
        switch (EFFECT_SEASON) {
        case 0:
            sourceIndex = 14;
            break;
        case 1:
        case 2:
            sourceIndex = 19;
            break;
        default:
            return;
        }
        break;
    case 0x40:
        animation = 3;
        if (EFFECT_SEASON != 2) {
            animation = 1;
        }
        sourceIndex = sourceBase + 6;
        break;
    default:
        if (Ovl11ItemHasFlag40((s16)id) == 1 &&
            (itemSeason = Ovl11ReadRecordTypeProperty00((s16)id), animation = 1, EFFECT_SEASON != itemSeason) &&
            EFFECT_AREA != 6) {
            animation = 1;
            sourceIndex = sourceBase + 6;
            if (itemSeason != 4) {
                animation = 3;
            }
        } else {
            animation = 1;
            sourceIndex = sourceBase + 6;
        }
        break;
    }
    effect->sprite = &gSpriteSourceBankBase + sourceIndex;
    effect->animation = animation;
    effect->tick = 0;
    effect->x = position.vx;
    effect->y = position.vy;
    effect->z = position.vz;
}
