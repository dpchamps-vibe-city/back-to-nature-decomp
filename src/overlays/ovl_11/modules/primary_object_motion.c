/*
 * Primary-object motion, ovl_11:CA708-CADBC. The frame integrator is the
 * sole caller of the side-slide probe. Their private offset/velocity tables
 * and complementary flag transitions form one motion protocol.
 * The collision service and surface grid remain independently shared services.
 */
#include "common.h"
#include "ovl11_primary_object_motion.h"
#include "game_types.h"
#include "sound_commands.h"
#include "ovl11_grid_cell_queries.h"
#include "ovl11_primary_object_state_change.h"
#include "ovl11_surface_height.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "pad_actuator_sequences.h"
#include "ovl11_coordinate_clamp.h"
#include "point_projection.h"
#include "ovl11_primary_object_collision_action.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "psyq/memory.h"
#include "surface_collision.h"

void Ovl11ProbePrimaryObjectMotion(void) {
    Ovl11PrimaryMotionObject *player;
    Ovl11CC7CCPosition position;
    Ovl11CC7CCPosition probe;
    s32 dx, dz;
    s32 surfaceResult;
    s32 zAhead;
    s32 collisionClass;
    s32 probeIndex;
    s32 clearSides;
    s32 xAhead;
    Ovl11SlideOffset *probeOffset;
    Ovl11SlideOffset *slideVelocity;

    probeOffset = 0;
    xAhead = 0;
    player = (Ovl11PrimaryMotionObject *)&gPrimaryObjectBase;
    position = player->position;
    position.vx = player->position.vx + player->velocity[0];
    position.vy = player->position.vy + player->velocity[1];
    position.vz = player->position.vz + player->velocity[2];
    zAhead = 0;
    switch (player->direction) {
    case 0:
        zAhead = -0x30;
        break;
    case 1:
        xAhead = -0x30;
        break;
    case 2:
        zAhead = 0x30;
        break;
    case 3:
        xAhead = 0x30;
        break;
    }
    position.vx += xAhead;
    position.vz += zAhead;
    collisionClass = Ovl11QueryPrimaryObjectCollisionAction((Ovl11CB114Object *)player, (s32 *)&position, 0x38);
    if (collisionClass == 0x3E7) {
        if ((gOvl11InteractionHitFlags & 0x20) &&
            (player->velocity[0] != 0 || player->velocity[2] != 0) &&
            Ovl11CheckDirectionalCellTransition((s32 *)&player->position, player->direction) != 0 &&
            player->mode == 4) {
            if (!(player->flags & 0x40000000)) {
                player->flags |= 0x40000000;
                PlaySoundEffect(6);
                if (player->flags & 0x100) {
                    ((struct Ovl11C907CStateView *)D_8006C838)->field_92D0 |= 0x01000000;
                    Ovl11ChangePrimaryObjectState((Ovl11C7010Object *)player, 5U, player->direction + 0x3F, -2);
                } else {
                    Ovl11ChangePrimaryObjectState((Ovl11C7010Object *)player, 5U, 0xA, -2);
                }
                player->velocity[0] = 0;
                player->velocity[2] = 0;
                player->velocity[1] = -0x50;
                return;
            }
        }
        collisionClass = 4;
        gOvl11InteractionHitFlags = 0;
        gOvl11InteractionHitRecordView = 0;
        goto probe_sides;
    }
probe_sides:
    if (gOvl11PrimaryHeldButtons & 0xF020) {
        clearSides = 0;
        probeOffset = &gOvl11MotionProbeOffsets[(player->direction & 1) * 2];
        surfaceResult = Ovl11ResolveSurfaceHeight((s32 *)&position, 0xE080);
        if (surfaceResult != 0) {
            if ((u32)gOvl11MotionHoldCount < 5U) {
                gOvl11MotionHoldCount += 1;
                player->velocity[0] = 0;
                player->velocity[2] = 0;
                return;
            }
        } else {
            if (collisionClass == 4) {
                player->velocity[0] = 0;
                player->velocity[2] = 0;
            }
            gOvl11MotionHoldCount = 0;
            return;
        }
        probeIndex = 0;
        do {
            probe = player->position;
            dx = probeOffset->x;
            dz = probeOffset->z;
            probe.vx += dx;
            probe.vz += dz;
            if (Ovl11ResolveSurfaceHeight((s32 *)&probe, 0xE080) == 0 &&
                Ovl11QueryPrimaryObjectCollisionAction((Ovl11CB114Object *)player, (s32 *)&probe, 0) != 4) {
                clearSides |= 1 << probeIndex;
            }
            probeIndex += 1;
            probeOffset++;
        } while (probeIndex < 2);
        if (surfaceResult != 0 && !(player->flags & 0x800000)) {
            player->velocity[0] = 0;
            player->velocity[2] = 0;
            slideVelocity = &gOvl11MotionSlideVelocities[(player->direction & 1) * 2];
            if (gOvl11PrimaryHeldButtons & 0x20) {
                if (clearSides == 1) {
                    player->velocity[0] = (s32)slideVelocity[0].x;
                    player->velocity[2] = (s32)slideVelocity[0].z;
                }
                if (clearSides == 2) {
                    player->velocity[0] = (s32)slideVelocity[1].x;
                    player->velocity[2] = (s32)slideVelocity[1].z;
                }
                if (clearSides == 3) {
                    player->velocity[0] = (s32)slideVelocity[1].x;
                    player->velocity[2] = (s32)slideVelocity[1].z;
                }
            }
        }
    }
}

void Ovl11UpdatePrimaryObjectMotion(void) {
    Ovl11PrimaryMotionObject *player;
    Ovl11MovementState *state;
    Ovl11CC7CCPosition position;
    Ovl11SlideOffset *offset;
    SVECTOR *camera;
    Ovl11CameraOffsetView *cameraOffset;
    s32 *screen;

    player = (Ovl11PrimaryMotionObject *)&gPrimaryObjectBase;
    if (!(player->flags & 2) && !(((Ovl11MovementState *)((u8 *)player - 0x51C8))->flags10 & 0x40000000)) {
        Ovl11ProbePrimaryObjectMotion();
    }
    D_80128C50 = *(s32 *)GetSurfaceHitPolygonAddress();
    position = player->position;
    if (player->flags & 0x40000000) {
        if (!(player->flags & 1)) {
            offset = &gOvl11MotionSlideOffsets[player->direction];
            if (player->mode == 0x3B) {
                /* The preceding bank is shared with the direction service. */
                offset = &((Ovl11SlideOffset *)((u8 *)gOvl11MotionSlideOffsets - 0x30))[player->direction];
            }
            if (player->mode == 0x4B) {
                offset = &gOvl11MotionSlideOffsets[player->direction];
            }
            player->velocity[0] += offset->x;
            player->velocity[2] += offset->z;
            SetSurfaceHeightTolerance(0x32);
        }
    }
    position.vx += player->velocity[0];
    position.vy += player->velocity[1];
    position.vz += player->velocity[2];
    if (player->flags & 0x40000000) {
        if (Ovl11ResolveSurfaceHeight((s32 *)&position, 0xC000) == 0) {
            RequestPadActuatorSequence(0, 2);
            player->velocity[1] = 0;
            player->flags &= ~0x40000000;
            if (player->flags & 0x100) {
                ((struct Ovl11C907CStateView *)D_8006C838)->field_92D0 &= ~0x01000000;
                Ovl11ChangePrimaryObjectState((Ovl11C7010Object *)&((Ovl11MovementState *)D_8006C838)->player, 1, 0, -2);
            }
        } else if (!(player->flags & 1)) {
            player->velocity[1] += 0x10;
            if (player->velocity[1] > 0x80) {
                player->velocity[1] = 0x80;
            }
        }
    } else {
        Ovl11ResolveSurfaceHeight((s32 *)&position, 0xC080);
    }
    SetSurfaceHeightTolerance(0xC8);
    player->position = position;
    state = (Ovl11MovementState *)D_8006C838;
    if (!(state->flagsC & 0x400000)) {
        ((Ovl11MovementCameraWork *)&D_8007AFF0)->coordinates[0] = (u16)state->player.position.vx;
        ((Ovl11MovementCameraWork *)&D_8007AFF0)->coordinates[1] = (u16)state->player.position.vy;
        ((Ovl11MovementCameraWork *)&D_8007AFF0)->coordinates[2] = (u16)state->player.position.vz;
    }
    player->velocity[0] = 0;
    player->velocity[2] = 0;
    if (state->flags10 & 0x08000000) {
        Ovl11ClampCoordinates((Ovl11SceneCoordinates *)D_800A03A4);
    }
    PushMatrix();
    screen = gOvl11MotionScreenPoint;
    camera = (SVECTOR *)D_800A03A4;
    ProjectPointToScreen(screen, camera);
    cameraOffset = (Ovl11CameraOffsetView *)camera;
    cameraOffset = (Ovl11CameraOffsetView *)((u8 *)cameraOffset - 0x20);
    cameraOffset->unk14 = 0xA0 - (u16)screen[0];
    cameraOffset->unk16 = 0x78 - (u16)screen[1];
    PopMatrix();
}
