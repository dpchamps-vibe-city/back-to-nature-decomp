#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

void ovl_11_func_800CA708(void);
s32 func_8001EF98(void);
void func_8001E334(s32);
s32 ovl_11_func_800BDB88(s32 *, s32);
void func_8001B2CC(s32, s32);
void ovl_11_func_800C70CC(Ovl11C7010Object *, u16, s32, s32);
void ovl_11_func_800DB054(Ovl11SceneCoordinates *);
s32 func_8001DFD4(s32 *, SVECTOR *);

void ovl_11_func_800CAADC(void) {
    Ovl11CA708Player *player;
    Ovl11MovementState *state;
    Ovl11CC7CCPosition position;
    Ovl11SlideOffset *offset;
    SVECTOR *camera;
    Ovl11CameraOffsetView *cameraOffset;
    s32 *screen;

    player = (Ovl11CA708Player *)&D_80071A00;
    if (!(player->flags & 2) && !(((Ovl11MovementState *)((u8 *)player - 0x51C8))->flags10 & 0x40000000)) {
        ovl_11_func_800CA708();
    }
    D_80128C50 = *(s32 *)func_8001EF98();
    position = player->position;
    if (player->flags & 0x40000000) {
      if (!(player->flags & 1)) {
        offset = &D_80123100[player->direction];
        if (player->mode == 0x3B) {
            offset = &((Ovl11SlideOffset *)((u8 *)D_80123100 - 0x30))[player->direction];
        }
        if (player->mode == 0x4B) {
            offset = &D_80123100[player->direction];
        }
        player->velocity[0] += offset->x;
        player->velocity[2] += offset->z;
        func_8001E334(0x32);
      }
    }
    position.vx += player->velocity[0];
    position.vy += player->velocity[1];
    position.vz += player->velocity[2];
    if (player->flags & 0x40000000) {
        if (ovl_11_func_800BDB88((s32 *)&position, 0xC000) == 0) {
            func_8001B2CC(0, 2);
            player->velocity[1] = 0;
            player->flags &= ~0x40000000;
            if (player->flags & 0x100) {
                ((struct Ovl11C907CStateView *)D_8006C838)->field_92D0 &= ~0x01000000;
                ovl_11_func_800C70CC((Ovl11C7010Object *)&((Ovl11MovementState *)D_8006C838)->player, 1, 0, -2);
            }
        } else if (!(player->flags & 1)) {
            player->velocity[1] += 0x10;
            if (player->velocity[1] > 0x80) {
                player->velocity[1] = 0x80;
            }
        }
    } else {
        ovl_11_func_800BDB88((s32 *)&position, 0xC080);
    }
    func_8001E334(0xC8);
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
        ovl_11_func_800DB054((Ovl11SceneCoordinates *)D_800A03A4);
    }
    PushMatrix();
    screen = D_80128B90;
    camera = (SVECTOR *)D_800A03A4;
    func_8001DFD4(screen, camera);
    cameraOffset = (Ovl11CameraOffsetView *)camera;
    cameraOffset = (Ovl11CameraOffsetView *)((u8 *)cameraOffset - 0x20);
    cameraOffset->unk14 = 0xA0 - (u16)screen[0];
    cameraOffset->unk16 = 0x78 - (u16)screen[1];
    PopMatrix();
}
