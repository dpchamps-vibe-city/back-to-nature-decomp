/*
 * Hour-selected ambient tint and forty screen-space falling/splash particles.
 * Setup selects a four-entry RGB table and particle mode; the scene tick gates
 * particle animation and tint interpolation separately. All storage is external,
 * including gScreenColorRamp, whose full 0x24-byte block ovl_15 saves/restores.
 * See ovl11_weather_effect.h and screen_color_ramp.h for public contracts;
 * notes/file-groupings.md records membership and the evidence-limited edges.
 */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "screen_color_ramp.h"
#include "ovl11_weather_effect.h"
#include "screen_fade.h"
#include "ovl11_clock_calendar.h"
#include "random.h"
#include "sprite_render.h"
#include "sprite_source.h"

void Ovl11UpdateWeatherParticles(void);
void Ovl11ClearWeatherParticle(Ovl11WeatherParticle *particle);
void Ovl11RespawnWeatherParticle(u8 *particle);
s32 ovl_11_func_800F5868(s32 x, s32 y);

void Ovl11UpdateWeatherEffect(void) {
    s32 hourClass;
    u16 spriteFlags;
    u8 *base;
    u8 *work;
    u8 *tail;

    spriteFlags = ((u16 *)((struct Ovl11SpriteWork *)&D_8007AFF0)->header)[4];
    if (!(spriteFlags & 0x800) && (((struct Ovl11SpriteWork *)&D_8007AFF0)->header[1] != 6)) {
        if (!(spriteFlags & 0x1000)) {
            Ovl11UpdateWeatherParticles();
        }
        base = (u8 *)D_8006C838;
        if ((*(s32 *)(base + 0xC)) & 0x02000000) {
            work = base + 0x8000;
            hourClass = Ovl11ClassifyClockHour(*(s16 *)(base + 0x44C0));
            if ((*(s32 *)(work + 0x6768)) != hourClass) {
                Ovl11SetScreenColorRampTarget(0x100, &gOvl11AmbientTintTable[hourClass * 3]);
                (*(s32 *)(work + 0x6768)) = hourClass;
            }
        }
        tail = (u8 *)D_8006C838;
        if (!((*(s32 *)(tail + 0xC)) & 0x10200000) && !(((u16 *)((struct Ovl11SpriteWork *)&D_8007AFF0)->header)[4] & 0x1000)) {
            Ovl11DrawAndStepScreenColorRamp();
        }
    }
}

void Ovl11SelectCurrentHourTint(void) {
    char *base;
    s32 hourClass;

    base = (char *)&D_8006C838;
    hourClass = Ovl11ClassifyClockHour(*(s16 *)(base + 0x44C0));
    Ovl11SetScreenColorRampTarget(1, &gOvl11AmbientTintTable[hourClass * 3]);
    base += 0x8000;
    *(s32 *)(base + 0x6768) = hourClass;
}

void Ovl11ResetWeatherParticles(void) {
    u8 *particle;
    s32 remaining;

    particle = gOvl11WeatherParticles;
    remaining = 0x27;
    do {
        Ovl11ClearWeatherParticle((Ovl11WeatherParticle *)particle);
        remaining -= 1;
        particle += 0xE;
    } while (remaining >= 0);
    gOvl11AmbientTintTable = (s32 *)gOvl11DefaultAmbientTintTable;
}

void Ovl11WeatherResetHook(void) {
}

void Ovl11ResetAmbientTintFromTable(void) {
    char *base;
    s32 *rgb = gOvl11AmbientTintTable;

    base = (char *)&D_8006C838;
    base += 0x8000;
    *(s32 *)(base + 0x6748) = rgb[0];
    *(s32 *)(base + 0x6750) = rgb[2];
    *(s32 *)(base + 0x674C) = rgb[1];
    *(s32 *)(base + 0x6754) = 0;
    *(s32 *)(base + 0x675C) = 0;
    *(s32 *)(base + 0x6758) = 0;
    *(s32 *)(base + 0x6764) = 0;
    *(s32 *)(base + 0x6760) = 0;
    *(s32 *)(base + 0x6768) = 0;
}

void Ovl11ClearWeatherParticle(Ovl11WeatherParticle *particle) {
    particle->x = 0;
    particle->y = 0;
    particle->animation = 0;
    particle->frame = 0;
    particle->age = 0;
    particle->startDelay = 0;
    particle->flags = 0x8000;
}

void Ovl11DrawAndStepScreenColorRamp(void) {
    struct ScreenColorRampState *ramp = &gScreenColorRamp;

    /* The original tests the sum, not whether any individual channel is set.
     * Drawing precedes stepping; fractional remainders are never snapped. */
    if ((ramp->rgbQ12[0] + ramp->rgbQ12[1] + ramp->rgbQ12[2]) != 0) {
        DrawScreenSubtractOverlay(ramp->rgbQ12[0], ramp->rgbQ12[1], ramp->rgbQ12[2]);
    }
    if (ramp->durationUpdates > ramp->elapsedUpdates) {
        ramp->elapsedUpdates = ramp->elapsedUpdates + 1;
        ramp->rgbQ12[0] = ramp->rgbQ12[0] + ramp->stepQ12[0];
        ramp->rgbQ12[1] = ramp->rgbQ12[1] + ramp->stepQ12[1];
        ramp->rgbQ12[2] = ramp->rgbQ12[2] + ramp->stepQ12[2];
    }
}

void Ovl11SetScreenColorRampTarget(s32 updateCount, const s32 targetRgbQ12[3]) {
    struct ScreenColorRampState *ramp = &gScreenColorRamp;

    ramp->elapsedUpdates = 0;
    ramp->durationUpdates = updateCount;
    ramp->stepQ12[0] = (targetRgbQ12[0] - ramp->rgbQ12[0]) / updateCount;
    ramp->stepQ12[1] = (targetRgbQ12[1] - ramp->rgbQ12[1]) / updateCount;
    ramp->stepQ12[2] = (targetRgbQ12[2] - ramp->rgbQ12[2]) / updateCount;
}

/* Particle byte offsets correspond to Ovl11WeatherParticle. Keep the explicit
 * signed/unsigned halfword views: x wraps unsigned while y is signed, and the
 * age word also drives the splash frame and lateral oscillation. */
void Ovl11UpdateWeatherParticle(u8 *particle) {
    s32 screenY;
    s32 yOrFlags;
    s32 nextY;
    s32 swayStep;
    s32 fallStep;
    s32 driftStep;
    s32 screenX;
    u32 flagTest;
    u16 swayPhase;
    s32 age;
    u32 flagsOrFrame;

    if ((*(s16 *)((u8 *)particle + 8)) >= 0) {
        if (!(D_8006C844 & 0x08000000)) {
            (*(s16 *)((u8 *)particle + 8)) = (s16)((u16)(*(s16 *)((u8 *)particle + 8)) - 1);
        }
    } else {
        flagTest = *(u16 *)(particle + 0xC);
        yOrFlags = flagTest & 0x4000;
        if (yOrFlags) {
            flagsOrFrame = flagTest;
            flagTest = flagsOrFrame & 0x1000;
            fallStep = 5;
            if (flagTest) {
                driftStep = 7;
            } else {
                fallStep = 1;
                driftStep = 0;
            }
            yOrFlags = *(s16 *)(particle + 2);
            if (flagsOrFrame & 0x2000) {
                nextY = yOrFlags + (fallStep * 2);
            } else {
                nextY = yOrFlags + fallStep;
            }
            (*(s16 *)(particle + 2)) = nextY;
            (*(u16 *)(particle + 0)) -= driftStep;
            swayPhase = *(u16 *)(particle + 0xA);
            swayStep = (swayPhase & 7) != 7;
            if (swayPhase & 8) {
                (*(u16 *)(particle + 0)) += swayStep;
            } else {
                (*(u16 *)(particle + 0)) -= swayStep;
            }
            if (!(D_8006C844 & 0x08000000)) {
                age = *(u16 *)((u8 *)particle + 0xA);
                if ((*(u16 *)((u8 *)particle + 0xC)) & 0x2000) {
                    if (age >= 0x37) {
                        goto respawn;
                    }
                    goto advance_age;
                }
                if (age >= 0x6E) {
                    goto respawn;
                }
                goto advance_age;
            }
            goto draw_particle;
        }
        flagsOrFrame = (u16)(*(u16 *)((u8 *)particle + 0xA)) >> 1;
        (*(s16 *)((u8 *)particle + 6)) = (s16)flagsOrFrame;
        if (!(D_8006C844 & 0x08000000)) {
            if ((s32)flagsOrFrame >= 3) {
respawn:
                Ovl11RespawnWeatherParticle(particle);
                return;
            }
advance_age:
            (*(u16 *)((u8 *)particle + 0xA)) = (u16)((*(u16 *)((u8 *)particle + 0xA)) + 1);
            goto draw_particle;
        }
        (*(s16 *)((u8 *)particle + 6)) %= 3;
draw_particle:
        screenX = (*(u16 *)((u8 *)particle + 0)) + D_800A0384.unk14;
        (*(u16 *)((u8 *)particle + 0)) = screenX;
        screenY = (u16)(*(s16 *)((u8 *)particle + 2)) + D_800A0384.unk16;
        (*(s16 *)((u8 *)particle + 2)) = screenY;
        if (ovl_11_func_800F5868((s32)(s16)screenX, (s32)(s16)screenY) != 0) {
            DrawSpriteFrameRects(gActiveRenderContext->field_120, (s32)&gSharedSpriteSource,
                (s32)(*(u8 *)((u8 *)particle + 4)), (s32)(u8)(*(s16 *)((u8 *)particle + 6)),
                (s16)(s32)(s16)(*(u16 *)((u8 *)particle + 0)), (s16)(s32)(*(s16 *)((u8 *)particle + 2)));
        }
    }
}

void Ovl11UpdateWeatherParticles(void) {
    s32 startClaimed;
    u32 particleIndex;
    u8 *particle;

    startClaimed = 0;
    particle = gOvl11WeatherParticles;
    particleIndex = 0;
    do {
        if ((*(s16 *)((u8 *)particle + 4)) != 0) {
            Ovl11UpdateWeatherParticle(particle);
        }
        /* Only the first due record starts immediately. Delay subsequent
         * falling records by three updates to stagger their appearance. */
        if ((*(s16 *)((u8 *)particle + 8)) == 0) {
            if (startClaimed == 0) {
                startClaimed = 1;
            } else if ((*(u16 *)((u8 *)particle + 0xC)) & 0x4000) {
                *(s16 *)(particle + 8) += 3;
            }
        }
        particleIndex += 1;
        particle += 0xE;
    } while (particleIndex < 0x28U);
}

void Ovl11RespawnWeatherParticle(u8 *particle) {
    u16 flags;
    s32 delayRange;
    s32 xRange;
    s32 yRange;
    s32 xBase;
    s32 yBase;
    s32 delayBase;

    flags = *(u16 *)(particle + 0xC);
    if (flags & 0x4000) {
        if (!(flags & 0x1000)) {
            delayRange = 0x69A;
            xRange = 0xF0;
            yRange = 0x3C;
            delayBase = 0x14;
            yBase = 0x5;
            xBase = 0xF;
        } else {
            delayRange = 0x4A;
            xRange = 0x12C;
            yRange = 0xB4;
            delayBase = 0xC;
            yBase = 0xA;
            xBase = 0x1E;
        }
    } else {
        delayRange = 0x19;
        xRange = 0xF0;
        yRange = 0xA0;
        delayBase = 0xF;
        yBase = 0x14;
        xBase = 0x14;
    }
    *(s16 *)(particle + 8) = Rand(delayRange & 0xFFFF) + delayBase;
    Rand(delayRange & 0xFFFF);
    *(s16 *)(particle + 0) = Rand(xRange) + xBase;
    Rand(delayRange & 0xFFFF);
    *(s16 *)(particle + 2) = Rand(yRange) + yBase;
    *(s16 *)(particle + 0xA) = 0;
}

void Ovl11SetWeatherParticleMode(s16 mode) {
    s16 modeIndex;
    s32 candidateIndex;
    s32 animationIndex;
    s32 variantFlags;
    u16 fallingFlags;
    u32 particleIndex;
    u8 *particle;

    modeIndex = mode;
    candidateIndex = modeIndex - 1;
    if ((u32)(candidateIndex & 0xFFFF) < 4U) {
        modeIndex = candidateIndex;
        animationIndex = modeIndex - 2;
        if (modeIndex < 2) {
            animationIndex = modeIndex;
        }
        gOvl11AmbientTintTable = gOvl11WeatherAmbientTintTable;
        Ovl11ResetAmbientTintFromTable();
        InitSpriteSourceData(&gOvl11WeatherSpriteSource, (SpriteDataHeader *)&gSharedSpriteData);
        particle = gOvl11WeatherParticles;
        particleIndex = 0;
        do {
            (*(s16 *)((u8 *)particle + 4)) = (s16)(animationIndex + 0x19);
            if (modeIndex == 1) {
                fallingFlags = (*(u16 *)((u8 *)particle + 0xC)) | 0x4000;
                (*(u16 *)((u8 *)particle + 0xC)) = fallingFlags;
                if (!(particleIndex & 3)) {
                    (*(u16 *)((u8 *)particle + 0xC)) = (u16)(fallingFlags | 0x2000);
                }
                if (!(particleIndex & 7)) {
                    (*(s16 *)((u8 *)particle + 4)) = 0;
                }
            }
            if (modeIndex == 3) {
                variantFlags = (*(u16 *)((u8 *)particle + 0xC)) | 0x4000;
                (*(u16 *)((u8 *)particle + 0xC)) = (u16)(variantFlags | 0x1000);
                if (!(particleIndex & 1)) {
                    (*(u16 *)((u8 *)particle + 0xC)) = (u16)(variantFlags | 0x3000);
                }
            }
            Ovl11RespawnWeatherParticle(particle);
            particleIndex += 1;
            particle += 0xE;
        } while (particleIndex < 0x28U);
    }
}
