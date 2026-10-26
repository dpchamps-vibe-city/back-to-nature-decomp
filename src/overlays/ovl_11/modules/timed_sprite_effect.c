/*
 * Embedded timed sprite effects, ovl_11:80107DD0..80107F18.
 * Object initializers, update loops and renderers share the same four-byte
 * state, commonly at object+0xA8 or object+0xE0. Ticking and drawing are
 * separate: the renderer traverses animation frames backwards in ten-tick
 * steps. The public header records the flag-gated tick's unusual result.
 */
#include "ovl11_timed_sprite_effect.h"
#include "game_types.h"
#include "sprite_source.h"
#include "sprite_render.h"

void Ovl11ResetTimedSpriteEffect(void *state) {
    Ovl11TimedSpriteEffect *effect = state;

    effect->animationId = -1;
    effect->ticksRemaining = 0;
}

void Ovl11StartTimedSpriteEffect(void *state, s16 animationId, s16 ticks) {
    Ovl11TimedSpriteEffect *effect = state;

    effect->animationId = animationId;
    effect->ticksRemaining = ticks;
}

s32 Ovl11TickTimedSpriteEffect(void *state) {
    Ovl11TimedSpriteEffect *effect = state;

    if (!(D_8006C844 & 0x08000000) && (effect->ticksRemaining != 0)) {
        effect->ticksRemaining = (s16) ((u16) effect->ticksRemaining - 1);
        return 0;
    }
    return 1;
}

void Ovl11DrawTimedSpriteEffect(void *state, s32 ot, s16 x, s16 y) {
    Ovl11TimedSpriteEffect *effect = state;
    s16 frameCount;
    s16 frame;

    if (effect->ticksRemaining != 0) {
        frameCount = GetSpriteAnimationFrameCount((SpriteSourceData *)&gSharedSpriteSource, effect->animationId);
        frame = (s16)(effect->ticksRemaining / 10) % frameCount;
        /* Preserve the original byte-sized draw selector, not the full ID. */
        DrawSpriteFrameRects(ot, (s32)&gSharedSpriteSource, ((u8 *)state)[0],
                     (frameCount - frame - 1) & 0xFF, x, y);
    }
}
