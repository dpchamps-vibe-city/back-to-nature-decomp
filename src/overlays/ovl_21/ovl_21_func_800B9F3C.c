#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sound_commands.h"

void ovl_21_func_800B9F3C(void) {
    u8 *base;
    s32 i;
    s32 j;
    s16 dx;
    s16 dz;
    s32 velocityX;
    s32 velocityZ;
    s16 speed;
    s32 life;
    s16 steps;
    s16 distance;
    s32 target;
    s32 quotient;
    s32 triple;
    s32 shotOffset;
    s32 lookup;
    s32 scanDisplacement;
    u16 *work;
    Ovl21SignedRecordView *record;
    UnkStruct800C0448 *state;
    Ovl21ProjectileView *shot;
    SVECTOR *position;

    for (i = 0; i < 6; i++) {
        if (D_800C0448_init.records[i].unk14 != 1) {
            D_800C0448_init.records[i].unk28 = 0;
        } else if (D_800C0448_init.records[i].unk28 == 1) {
            state = &((UnkStruct800C0448 *)D_800C0448)[i];
            if (state->unk18 != 2) {
                state->unk18 = 2;
                D_800C0448_init.records[i].unk28 = 0;
            } else if (!(((u16 *)D_800C0448_init.records[i].unk30)[1] & 0x100)) {
                D_800C0448_init.records[i].unk28 = 0;
            } else {
                if (i < 3) {
                    if (state->unk2C < 3) {
                        D_800C0448_init.records[i].unk28 = 0;
                        continue;
                    }
                } else if (state->unk2C >= 3) {
                    D_800C0448_init.records[i].unk28 = 0;
                    continue;
                }
                for (j = 0;
                    (base = (u8 *)D_800C0448),
                    j < *(s16 *)((u8 *)base + 0x64A) &&
                    ((scanDisplacement = -(j * sizeof(Ovl21InitSubstate) + i * 0x108)),
                    ((UnkStruct800C0448 *)(base - scanDisplacement))->unk34 != 0);
                    j++) {
                }
                if (j != *(s16 *)((u8 *)base + 0x64A)) {
                    record = &((Ovl21SignedRecordView *)base)[i];
                    record->unk18 = 4;
                    D_800C0448_init.records[i].unk28 = 0;
                    dx = ((Ovl21SignedRecordView *)base)[record->unk2C].unk1A - record->unk1A;
                    dz = ((Ovl21SignedRecordView *)base)[record->unk2C].unk1E - record->unk1E;
                    speed = record->unk24 * 400 / 3000;
                    distance = SquareRoot0(dx * dx + dz * dz);
                    life = (s16)(*(s16 *)((u8 *)base + 0x652) * 400 / speed);
                    steps = distance / speed;
                    velocityX = dx / steps;
                    velocityZ = dz / steps;
                    shotOffset = j * sizeof(Ovl21InitSubstate);
                    shotOffset += i * 0x108;
                    lookup = -shotOffset;
                    work = (u16 *)(base - lookup);
                    ((UnkStruct800C0448 *)work)->unk34 = 1;
                    position = (SVECTOR *)((u8 *)(D_800C0448 + 0x20) + shotOffset);
                    position->vx = record->unk1A;
                    position->vy = -350;
                    position->vz = record->unk1E;
                    work = (u16 *)&record->unk2C;
                    quotient = (s16)((s16)*work / 3);
                    triple = quotient + quotient + quotient;
                    target = (s16)*work - triple;
                    target = D_800BCC24[i * 3 + target];
                    shot = (Ovl21ProjectileView *)((u8 *)D_800C0448 + shotOffset);
                    shot->steps = steps;
                    shot->life = life;
                    shot->duration = life;
                    shot->kind = target;
                    setVector((SVECTOR *)((u8 *)(D_800C0448 + 0x24) + shotOffset), velocityX, 0, velocityZ);
                    *(s16 *)((u8 *)base + 0x984) = PlaySoundEffect(0x40);
                }
            }
        }
    }
}
