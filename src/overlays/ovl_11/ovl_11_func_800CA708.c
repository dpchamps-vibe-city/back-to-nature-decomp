#include "common.h"
#include "game_types.h"
#include "sound_commands.h"

s32 ovl_11_func_800CB114(Ovl11CB114Object *, s32 *, s16);
s32 ovl_11_func_800BF4B4(s32 *, s32);
void ovl_11_func_800C70CC(Ovl11C7010Object *, s32 arg1, s32, s32);
s32 ovl_11_func_800BDB88(s32 *, s32);

void ovl_11_func_800CA708(void) {
    Ovl11CA708Player *player;
    Ovl11CC7CCPosition pos;
    Ovl11CC7CCPosition probe;
    s32 dx, dz;
    s32 temp_v0;
    s32 var_a3;
    s32 var_s2;
    s32 var_s2_2;
    s32 var_s4;
    s32 var_t0;
    Ovl11SlideOffset *var_s0;
    Ovl11SlideOffset *temp_a1;

    var_s0 = 0;
    var_t0 = 0;
    player = (Ovl11CA708Player *)&D_80071A00;
    pos = player->position;
    pos.vx = (player->position.vx) + (player->velocity[0]);
    pos.vy = (player->position.vy) + (player->velocity[1]);
    pos.vz = (player->position.vz) + (player->velocity[2]);
    var_a3 = 0;
    switch (player->direction) { /* irregular */
    case 0:
        var_a3 = -0x30;
        break;
    case 1:
        var_t0 = -0x30;
        break;
    case 2:
        var_a3 = 0x30;
        break;
    case 3:
        var_t0 = 0x30;
        break;
    }
    pos.vx += var_t0;
    pos.vz += var_a3;
    var_s2 = ovl_11_func_800CB114((Ovl11CB114Object *)player, (s32 *)&pos, 0x38);
    if (var_s2 == 0x3E7) {
        if ((D_80128C54 & 0x20) && (((player->velocity[0]) != 0) || ((player->velocity[2]) != 0)) && (ovl_11_func_800BF4B4((s32 *)&player->position, (s32) (player->direction)) != 0) && ((player->mode) == 4)) {
            if (!((player->flags) & 0x40000000)) {
                (player->flags) = (s32) ((player->flags) | 0x40000000);
                PlaySoundEffect(6);
                if ((player->flags) & 0x100) {
                    ((struct Ovl11C907CStateView *)D_8006C838)->field_92D0 = (s32) (((struct Ovl11C907CStateView *)D_8006C838)->field_92D0 | 0x01000000);
                    ovl_11_func_800C70CC((Ovl11C7010Object *)player, 5U, (player->direction) + 0x3F, -2);
                } else {
                    ovl_11_func_800C70CC((Ovl11C7010Object *)player, 5U, 0xA, -2);
                }
                (player->velocity[0]) = 0;
                (player->velocity[2]) = 0;
                (player->velocity[1]) = -0x50;
                return;
            }
        }
        var_s2 = 4;
        D_80128C54 = 0;
        D_80128C4C_dispatch = 0;
        goto block_25;
    }
block_25:
    if (D_801231F4 & 0xF020) {
        var_s4 = 0;
        var_s0 = &D_80123250[(player->direction & 1) * 2];
        temp_v0 = ovl_11_func_800BDB88((s32 *)&pos, 0xE080);
        if (temp_v0 != 0) {
            if ((u32) D_80128C68 < 5U) {
                D_80128C68 += 1;
                (player->velocity[0]) = 0;
                (player->velocity[2]) = 0;
                return;
            }
        } else {
            if (var_s2 == 4) {
                (player->velocity[0]) = 0;
                (player->velocity[2]) = 0;
            }
            D_80128C68 = 0;
            return;
        }
        var_s2_2 = 0;
        do {
            probe = player->position;
            dx = var_s0->x;
            dz = var_s0->z;
            probe.vx += dx;
            probe.vz += dz;
            if ((ovl_11_func_800BDB88((s32 *)&probe, 0xE080) == 0) && (ovl_11_func_800CB114((Ovl11CB114Object *)player, (s32 *)&probe, 0) != 4)) {
                var_s4 |= 1 << var_s2_2;
            }
            var_s2_2 += 1;
            var_s0++;
        } while (var_s2_2 < 2);
        if ((temp_v0 != 0) && !((player->flags) & 0x800000)) {
            (player->velocity[0]) = 0;
            (player->velocity[2]) = 0;
            temp_a1 = &D_80123270[(player->direction & 1) * 2];
            if (D_801231F4 & 0x20) {
                if (var_s4 == 1) {
                    (player->velocity[0]) = (s32) temp_a1[0].x;
                    (player->velocity[2]) = (s32) temp_a1[0].z;
                }
                if (var_s4 == 2) {
                    (player->velocity[0]) = (s32) temp_a1[1].x;
                    (player->velocity[2]) = (s32) temp_a1[1].z;
                }
                if (var_s4 == 3) {
                    (player->velocity[0]) = (s32) temp_a1[1].x;
                    (player->velocity[2]) = (s32) temp_a1[1].z;
                }
            }
        }
    }
}
