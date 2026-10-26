#include "common.h"
#include "game_types.h"
#include "psyq/sys/types.h"
#include "psyq/libgte.h"
#include "psyq/stdlib.h"
#include "random.h"
#include "ovl23_direction_movement.h"
#include "ovl23_record_control_fields.h"

void ovl_23_func_800B989C(Ovl23Func800BB0C8Arg *arg0);

void ovl_23_func_800B8FA4(void) {
    s32 i;
    s16 dx;
    s16 dy;
    s16 distance;
    s16 state;
    s16 direction;
    s16 mode;
    struct Ovl23ActorDecisionView *base;
    struct Ovl23ActorDecision *actor;
    struct Ovl23ActorDecision *actors;
    struct Ovl23ActorDecisionView *stateBase;
    u16 *table;
    struct Ovl23ActorDecisionView *base2;
    struct Ovl23ActorDecision *actor2;
    s32 offset;
    struct Ovl23ActorDecisionView *window;
    s32 walkingState;
    struct Ovl23ActorDecisionView *modeRoot;
    struct Ovl23ActorDecision *decisionActor;
    s16 sampledDirection;

    i = 0;
    table = (u16 *)&gOvl23RecordAnchorY;
    base = &Ovl23_D_800BF87C_decision;
    actor = base->actors;
    for (; i < 6; i++) {
        if (actor->unk2 == 1) {
            if (actor->unk8 > 0) {
                actor->unk8--;
            } else if (base->unk14 == (base->unk14 / base->unk3AC) * base->unk3AC) {
                dx = base->actors[i].unk14 / 4096 - base->records[i].unkC / 4096;
                dy = base->actors[i].unk18 / 4096 - table[i];
                distance = abs(SquareRoot0(dx * dx + dy * dy));
                if (abs(dx) < actor->unkA) {
                    if (distance <= base->unk39E) {
                        Ovl23SetActorControl((Ovl23Func800BB0C8Arg *)actor, 2,
                            Ovl23DirectionFromRecordAnchor((Ovl23PositionedRecord *)actor), base->unk3AC);
                    } else {
                        Ovl23SetActorControl((Ovl23Func800BB0C8Arg *)actor, 1,
                            Ovl23ChooseRecordDirection((Ovl23PositionedRecord *)actor), base->unk3AC);
                    }
                } else {
                    state = Rand(8) + 3;
                    if (state != 3) {
                        direction = (Rand(4) << 1) | 1;
                    } else {
                        direction = Rand(5) + 4;
                        direction = direction == 8 ? 0 : direction;
                    }
                    Ovl23SetActorControl((Ovl23Func800BB0C8Arg *)actor, state, direction, 60);
                }
            }
        }
        actor++;
    }
    walkingState = 3;
    base2 = (struct Ovl23ActorDecisionView *)Ovl23_D_800BF87C_draw;
    actor2 = base2->actors;
    actors = actor2;
    offset = 0;
    decisionActor = actor2;
    window = base2;
    modeRoot = window;
    for (i = 0; i < 6; i++, window = (struct Ovl23ActorDecisionView *)((u8 *)window + 0x50)) {
        if (window->actors[0].unk4 == walkingState &&
            window->actors[0].unk8 == (window->actors[0].unk8 / base2->unk3AE) * base2->unk3AE) {
            sampledDirection = Rand(5) + 4;
            direction = sampledDirection;
            sampledDirection = 8;
            direction = direction == sampledDirection ? 0 : direction;
            Ovl23SetActorControl((Ovl23Func800BB0C8Arg *)decisionActor, window->actors[0].unk4,
                direction, window->actors[0].unk8);
        }
        stateBase = (struct Ovl23ActorDecisionView *)(offset + (u32)modeRoot);
        mode = stateBase->actors[0].unk2;
        stateBase = (struct Ovl23ActorDecisionView *)actors;
        if (mode == 1) {
            Ovl23StepRecordInDirection((Ovl23PositionedRecord *)actor2);
        } else if (mode == walkingState) {
            ovl_23_func_800B989C((Ovl23Func800BB0C8Arg *)(offset + (u32)stateBase));
        }
        actor2++;
        offset += 0x50;
        decisionActor++;
    }
}
