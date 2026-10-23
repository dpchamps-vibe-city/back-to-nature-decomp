#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


int ccos (int a);

typedef struct {
    u8 pad_00[0x34];
    s32 unk34;
    u16 unk38;
    u16 unk3A;
    s16 unk3C;
    s16 unk3E;
    u16 unk40;
    u16 unk42;
    u16 unk44;
    u16 pad_46;
    u16 unk48;
    u16 unk4A;
    u16 unk4C;
} Ovl21Func800BA300Rec;

/* Steps the three 0x4C-byte projectile states of each of the six
 * D_800C0448 records: counts down unk3A, advances the position by the
 * velocity, recomputes the vertical velocity from a cosine, and clears the
 * slot's active word once unk3A passes -0x3B.
 *
 * Three constructs are here for the compiler, not for behaviour:
 *
 * - "raw" is assigned inside the outer loop, and the dead "base = raw;" at the
 *   end of the outer body stretches raw's in-loop lifetime. loop.c then rates
 *   moving its LO_SUM out as desirable (29 x savings x lifetime >= insns), and
 *   it shares the prologue HIGH that GCSE inserted (lui s6 / addiu s7,s6).
 *   Without the dead binding the LO_SUM stays in the loop.
 *
 * - "flags = (s32 *)((s32)flags | zero);" is a no-op that keeps the load's
 *   and the store's flags + offset apart. GCSE's cprop turns zero into 0 but
 *   validate_replace_rtx only simplifies PLUS/MINUS, so the IOR stays a set
 *   of flags. PRE then sees the expression killed between the load and the
 *   store and leaves the store's add in place, and loop.c sees flags as
 *   non-invariant and builds no giv to combine and strength-reduce. cse2
 *   folds the IOR to a self-copy and deletes it. Without it the two adds are
 *   merged and reduced into a pointer stepped by 0x4C.
 *
 * - The record base is spelled D_800C0458 - 0x10, a different HIGH from
 *   D_800C0448, so it gets its own lui in the outer body as in the target.
 *
 * Mechanism: notes/research/ovl_21_func_800BA300-shared-high.md. */
void ovl_21_func_800BA300(void) {
    s16 steps;
    s16 life;
    s32 active;
    s32 angle;
    s32 offset;
    s32 slot;
    s32 zero;
    s32 group;
    s32 *flags;
    Ovl21Func800BA300Rec *state;
    Ovl21Func800BA300Rec *record;
    u16 *velocity;
    u8 *raw;
    u8 *base;

    zero = 0;
    group = 0;
    do {
        slot = 0;
        raw = (u8 *) D_800C0448;
        if ((*(s16 *) (raw + 0x64A)) > 0) {
            base = (u8 *) D_800C0458 - 0x10;
            offset = group * 0x108;
            state = (Ovl21Func800BA300Rec *) (base + offset);
            flags = (s32 *) (base + 0x34);
            do {
                active = *(s32 *)(offset + (s32)flags);
                if (active != 0) {
                    flags = (s32 *)((s32)flags | zero);
                    life = --state->unk3A;
                    state->unk40 = (u16) (state->unk40 + state->unk48);
                    state->unk42 += state->unk4A;
                    steps = state->unk3E;
                    state->unk44 = (u16) (state->unk44 + state->unk4C);
                    angle = state->unk3C;
                    if (steps != 0) {
                        angle = (s32) ((angle - life) << 0xC) / (s32) (steps * 2);
                        state->unk4A = (u16)(angle < 0x800
                            ? -(ccos(angle) * 0x18 / 0x1000)
                            : state->unk4A + 0xC);
                        record = (Ovl21Func800BA300Rec *) ((u8 *) D_800C0448 + offset);
                        if ((s16) record->unk42 >= 0) {
                            velocity = (u16 *) ((u8 *) D_800C0448 + 0x48 + offset);
                            record->unk42 = 0;
                            velocity[0] = 0;
                            velocity[1] = 0;
                            velocity[2] = 0;
                            record->unk38 = 1;
                        }
                        if ((s16) record->unk3A < -0x3B) {
                            *(s32 *)(offset + (s32)flags) = 0;
                        }
                    }
                }
                offset += 0x4C;
                state = (Ovl21Func800BA300Rec *) ((u8 *) state + 0x4C);
            } while (++slot < (*(s16 *) (base + 0x64A)));
        }
        base = raw;
        group += 1;
    } while (group < 6);
}
