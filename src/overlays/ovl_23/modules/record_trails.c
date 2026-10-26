/*
 * record_trails — reconstructed translation unit.
 *
 * Two bounded record-trail rings, with reset, append and drawing services.
 * The 54 decision-record nodes and 12 actor nodes have private cursors and
 * share the same marker-sprite protocol.
 *
 * X positions are Q12; lane Y is a pixel coordinate and -1 disables a node.
 * Drawing uses the scene camera and the shared marker sprite without
 * advancing either ring. Interface: ovl23_record_trails.h.
 * Membership and boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl23_record_trails.h"
#include "psyq/abs.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void Ovl23ResetRecordTrails(void) {
    s16 i;

    for (i = 0; i < 0x36; i++) {
        ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->decisionNodes[i].xQ12 = 0x17000;
        ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->decisionNodes[i].laneY = -1;
        if (i < 6) {
            ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->decisionNodes[i].laneY = ((u16 *)&gOvl23RecordAnchorY)[i];
        }
    }
    ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->decisionNext = 6;
    for (i = 0; i < 0xC; i++) {
        ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->actorNodes[i].xQ12 = 0xFFFE5000;
        ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->actorNodes[i].laneY = -1;
        if (i < 6) {
            ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->actorNodes[i].laneY = ((u16 *)&gOvl23RecordAnchorY)[i];
        }
    }
    ((Ovl23TrailWork *)Ovl23_D_800BF87C_draw)->actorNext = 6;
}

void Ovl23PushDecisionTrailNode(s32 xQ12, s16 laneY, s16 state) {
    if (gOvl23RecordTrails.decisionNext >= 0x36) {
        gOvl23RecordTrails.decisionNext = 0;
    }
    gOvl23RecordTrails.decisionNodes[gOvl23RecordTrails.decisionNext].xQ12 = xQ12;
    gOvl23RecordTrails.decisionNodes[gOvl23RecordTrails.decisionNext].laneY = laneY;
    gOvl23RecordTrails.decisionNodes[gOvl23RecordTrails.decisionNext].state = state;
    gOvl23RecordTrails.decisionNext++;
}

void Ovl23PushActorTrailNode(s32 xQ12, s16 laneY, s16 state) {
    if (gOvl23RecordTrails.actorNext >= 0xC) {
        gOvl23RecordTrails.actorNext = 0;
    }
    gOvl23RecordTrails.actorNodes[gOvl23RecordTrails.actorNext].xQ12 = xQ12;
    gOvl23RecordTrails.actorNodes[gOvl23RecordTrails.actorNext].laneY = laneY;
    gOvl23RecordTrails.actorNodes[gOvl23RecordTrails.actorNext].state = state;
    gOvl23RecordTrails.actorNext++;
}

/* Cull outside half the screen width and truncate signed Q12 X toward zero.
 * Preserve the repeated frame product and the zero-valued comparison term:
 * CSE/flow/combine see them at different stages, so simplifying them changes
 * operand order and live ranges even though the final arithmetic is equal.
 * Mechanism: notes/research/ovl_23_func_800B954C-variable-partition.md. */
void Ovl23DrawRecordTrails(void) {
    s16 i;
    s32 frame;
    s32 kind;
    s16 x;
    s32 position;
    s32 roundedCamera;
    s32 distance;
    s32 test;
    s32 cam;
    s32 delta;
    ObjectState *sprite;
    u8 *base;
    Ovl23TrailWork *walk;

    i = 0;
    base = Ovl23_D_800BF87C_draw;
    sprite = (ObjectState *)(base + 0x3B4);
    walk = (Ovl23TrailWork *)base;
    do {
        kind = i * 8;
        if (walk->actorNodes[0].laneY != -1) {
            position = ((Ovl23TrailWork *)(base + kind))->actorNodes[0].xQ12;
            kind = walk->actorNodes[0].state;
            frame = kind;
            test = (s16)(i / 12);
            roundedCamera = i - test * 12;
            roundedCamera = roundedCamera < 6;
            roundedCamera ^= 1;
            test = roundedCamera * 8;
            test = frame + roundedCamera * 8;
            test = (u32)test << 16;
            frame = test >> 16;
            kind = position;
            if (position < 0) {
                kind = position + 4095;
            }
            distance = kind >> 12;
            cam = *(s32 *)(base + 0x10);
            delta = distance - cam / 4096;
            distance = abs(delta);
            distance = HWD0 / 2 < distance;
            if (!distance) {
                distance = (position | cam) < 0;
                x = HWD0 / 2 - delta + (distance >> 1);
                SelectSpriteAnimation(sprite, 4);
                SelectSpriteAnimationFrame(sprite, frame & 0xFF);
                DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0xD0, (SpriteSourceData *)sprite, x, walk->actorNodes[0].laneY);
            }
        }
        walk = (Ovl23TrailWork *)((u8 *)walk + 8);
        i++;
        test = i < 12;
    } while (test);
    i = 0;
    base = Ovl23_D_800BF87C_draw;
    sprite = (ObjectState *)(base + 0x3B4);
    walk = (Ovl23TrailWork *)base;
    do {
        distance = i * 8;
        if (walk->decisionNodes[0].laneY != -1) {
            position = ((Ovl23TrailWork *)(base + distance))->decisionNodes[0].xQ12;
            kind = walk->decisionNodes[0].state;
            kind ^= 6;
            frame = kind == 0;
            test = (s16)(i / 12);
            roundedCamera = i - test * 12;
            roundedCamera = roundedCamera < 6;
            roundedCamera ^= 1;
            test = roundedCamera * 2;
            frame |= test;
            kind = position;
            if (position < 0) {
                kind = position + 4095;
            }
            distance = kind >> 12;
            cam = *(s32 *)(base + 0x10);
            delta = distance - cam / 4096;
            distance = abs(delta);
            distance = HWD0 / 2 < distance;
            if (!distance) {
                distance = (position | cam) < 0;
                x = HWD0 / 2 - delta + (distance >> 1);
                SelectSpriteAnimation(sprite, 3);
                SelectSpriteAnimationFrame(sprite, frame & 0xFF);
                DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0xD0, (SpriteSourceData *)sprite, x, walk->decisionNodes[0].laneY);
            }
        }
        walk = (Ovl23TrailWork *)((u8 *)walk + 8);
        i++;
        test = i < 54;
    } while (test);
}
