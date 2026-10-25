/*
 * Two-slot actor behavior states, original ovl_19:800B998C-800BA25C.
 * The dispatcher owns states 3..11 and their position stepper. State changes
 * go through actor_controls; shared scene work and the public control API
 * remain outside this unit. Original signed/unsigned record views are kept.
 */
#include "common.h"
#include "game_types.h"
#include "ovl19_actor_behavior_states.h"
#include "ovl19_actor_geometry.h"
#include "sound_commands.h"
#include "ovl19_actor_controls.h"
#include "random.h"

void Ovl19UpdateDirectionChoice(s16 *record);
void Ovl19ActorState4NoOp(s16 *record);
s32 Ovl19StepUntilCountdownEnds(Ovl19ActorCountdownView *record);
void Ovl19ActorState6Step(Ovl19ActorMotionView *record);
void Ovl19ActorState7Step(Ovl19ActorMotionView *record);
void Ovl19ActorState8Step(Ovl19ActorMotionView *record);
void Ovl19StepAfterDoubleThreshold(Ovl19ActorMotionView *record);
void Ovl19StepWithTimedSound(Ovl19ActorMotionView *record);
void Ovl19StepWhileValueBelowLimit(Ovl19ActorLimitView *record);
void Ovl19StepActorPosition(Ovl19ActorMotionView *record);

s32 ovl_19_func_800BA5B4(s16 *record, s16 *other);
s32 ovl_19_func_800BA65C(Ovl19Func800BAC40Arg *record);

void Ovl19UpdateActorBehaviors(void) {
    s16 *record;
    s16 state;
    s32 slot;

    for (slot = 0; slot < 2; slot++) {
        record = (s16 *) ((u8 *) gOvl19ActorPairState + (slot * 0x48));
        state = *(s16 *) ((u8 *) record + 2);
        switch (state) {
        case 0:
        case 1:
        case 2:
            break;
        case 3:
            Ovl19UpdateDirectionChoice(record);
            break;
        case 4:
            Ovl19ActorState4NoOp(record);
            break;
        case 5:
            Ovl19StepUntilCountdownEnds((Ovl19ActorCountdownView *) record);
            break;
        case 6:
            Ovl19ActorState6Step((Ovl19ActorMotionView *) record);
            break;
        case 7:
            Ovl19ActorState7Step((Ovl19ActorMotionView *) record);
            break;
        case 8:
            Ovl19ActorState8Step((Ovl19ActorMotionView *) record);
            break;
        case 9:
            Ovl19StepAfterDoubleThreshold((Ovl19ActorMotionView *) record);
            break;
        case 10:
            Ovl19StepWithTimedSound((Ovl19ActorMotionView *) record);
            break;
        case 11:
            Ovl19StepWhileValueBelowLimit((Ovl19ActorLimitView *) record);
            break;
        }
        /* Handlers see the pre-decrement countdown, even after a transition. */
        if ((*(s16 *) ((u8 *) record + 6)) > 0) {
            (*(s16 *) ((u8 *) record + 6))--;
        }
    }
}

void Ovl19UpdateDirectionChoice(s16 *record) {
    s16 *other;
    s16 *base;
    s16 *work;
    s16 sum;
    s16 limit;
    s16 direction;
    s32 band;
    s32 roll;
    s16 flag;
    s32 i;

    other = gOvl19SceneWork + 8;
    base = other;
    if (record == other) {
        other = record + 0x24;
    }
    work = base - 8;
    if ((record[3] == (record[3] / work[0xCA]) * work[0xCA]) || (record[3] == work[0xBC])) {
        roll = (s32)Rand(100);
        flag = record[4] * 100 / 255 < roll;
        band = ovl_19_func_800BA5B4(record, other);
        sum = 0;
        limit = (s16)Rand(100);
        for (i = 0; i < 4; i++) {
            sum = (s16)(sum + gOvl19DirectionWeights[i + band * 4 + flag * 16]);
            if (sum >= limit) {
                direction = (s16)(record[2] + i);
                direction %= 4;
                Ovl19SetActorControl((Ovl19Func800BAC40Arg *)record, record[1], direction, record[3]);
                break;
            }
        }
    }
    if (record[3] == (record[3] / 2) * 2) {
        if (ovl_19_func_800BA5B4(record, other) == 0 &&
            Ovl19ActorPlanarDistance((Ovl19ActorPlanarPosition *)record,
                                     (Ovl19ActorPlanarPosition *)other) < 201) {
            if (record[4] >= 240 && Rand(128) >= 111U) {
                gOvl19SceneWork[0x101] = PlaySoundEffect(0x3B);
                Ovl19SetActorControl((Ovl19Func800BAC40Arg *)record, 4, record[2], 30);
            } else {
                Ovl19SetActorControl((Ovl19Func800BAC40Arg *)record, 2, record[2], record[3]);
            }
        } else if (ovl_19_func_800BA65C((Ovl19Func800BAC40Arg *)record) == 1) {
            if ((s32)Rand(128) < record[4]) {
                direction = (s16)(record[2] + ovl_19_func_800BA5B4(record, other));
                Ovl19SetActorControl((Ovl19Func800BAC40Arg *)record, 2, (s16)(direction % 4), record[3]);
            } else if (Rand(128) < 111U) {
                Ovl19SetActorControl((Ovl19Func800BAC40Arg *)record, 2, record[2], record[3]);
            }
        }
        if (record[1] == 3) {
            Ovl19StepActorPosition((Ovl19ActorMotionView *)record);
        }
    }
}

/* State 4 deliberately does nothing; its countdown still ticks in the driver. */
void Ovl19ActorState4NoOp(s16 *record) {
}

/* The dispatcher ignores this helper's result, including its zero-count path. */
s32 Ovl19StepUntilCountdownEnds(Ovl19ActorCountdownView *record) {
    Ovl19StepActorPosition((Ovl19ActorMotionView *)record);
    if (record->countdown == 0) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)record, 2, record->direction, 0);
    } else {
        return record->countdown;
    }
}

/* Three distinct original entry points share this exact countdown policy.
 * Keep them separate: the dispatcher and jump table preserve their identities. */
void Ovl19ActorState6Step(Ovl19ActorMotionView *record) {
    if ((gOvl19StepStartTicks - 0x1E) >= (*(s16 *) ((u8 *) record + 6))) {
        Ovl19StepActorPosition(record);
    }
    if ((*(s16 *) ((u8 *) record + 6)) == 0) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *) record, 2, (s32) record->direction, 0);
    }
}

void Ovl19ActorState7Step(Ovl19ActorMotionView *record) {
    if ((gOvl19StepStartTicks - 0x1E) >= (*(s16 *) ((u8 *) record + 6))) {
        Ovl19StepActorPosition(record);
    }
    if ((*(s16 *) ((u8 *) record + 6)) == 0) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *) record, 2, (s32) record->direction, 0);
    }
}

void Ovl19ActorState8Step(Ovl19ActorMotionView *record) {
    if ((gOvl19StepStartTicks - 0x1E) >= (*(s16 *) ((u8 *) record + 6))) {
        Ovl19StepActorPosition(record);
    }
    if ((*(s16 *) ((u8 *) record + 6)) == 0) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *) record, 2, (s32) record->direction, 0);
    }
}

void Ovl19StepAfterDoubleThreshold(Ovl19ActorMotionView *record) {
    s16 countdown;

    countdown = *(s16 *) ((u8 *) record + 6);
    if ((countdown < (gOvl19DoubleThresholdTicks - 0x1E)) && (countdown < (gOvl19DoubleThresholdTicks - 0x3C))) {
        Ovl19StepActorPosition(record);
    }
    if ((*(s16 *) ((u8 *) record + 6)) == 0) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *) record, 2, (s32) record->direction, 0);
    }
}

void Ovl19StepWithTimedSound(Ovl19ActorMotionView *record) {
    s16 countdown;

    countdown = *(s16 *) ((u8 *) record + 6);
    if ((countdown < (gOvl19SceneWork[0xC9] - 0x1E)) && (countdown < (gOvl19SceneWork[0xC9] - 0x2D))) {
        Ovl19StepActorPosition(record);
    }
    if ((*(s16 *) ((u8 *) record + 6)) == 0) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *) record, 2, (s32) record->direction, 0);
    }
    if ((*(s16 *) ((u8 *) record + 6)) == (gOvl19SceneWork[0xC9] - 0x1E)) {
        gOvl19SceneWork[0x101] = PlaySoundEffect(0x3C);
    }
}

void Ovl19StepWhileValueBelowLimit(Ovl19ActorLimitView *record) {
    if (record->value12 < -10) {
        Ovl19StepActorPosition((Ovl19ActorMotionView *)record);
    } else {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *)record, 2, record->direction, record->countdown);
    }
}

/* Scale the base rate for the state, choose one of four planar directions,
 * and store the wrapped low halfwords. There is no motion for other directions. */
void Ovl19StepActorPosition(Ovl19ActorMotionView *record) {
    s16 stateIndex;
    s16 direction;
    s32 delta0;
    s32 step;
    s32 delta1;

    stateIndex = record->state - 3;
    step = 0;
    switch (stateIndex) {
    case 2:
        step = gOvl19SceneWork[0xB8] * gOvl19SceneWork[0xCB] / 10;
        break;
    case 4:
        step = gOvl19SceneWork[0xB8] * gOvl19SceneWork[0xCC] * gOvl19SceneWork[0xCD] / 100;
        break;
    case 5:
        step = gOvl19SceneWork[0xB8] * gOvl19SceneWork[0xCC] * gOvl19SceneWork[0xCE] / 100;
        break;
    case 3:
    case 6:
        step = gOvl19SceneWork[0xB8] * gOvl19SceneWork[0xCC] / 10;
        break;
    case 7:
        step = gOvl19SceneWork[0xB8] * gOvl19SceneWork[0xCC] * gOvl19SceneWork[0xCF] / 100;
        break;
    case 0:
    case 8:
        step = gOvl19BaseActorStep;
        break;
    }
    delta1 = 0;
    direction = record->direction;
    delta0 = 0;
    switch (direction) {
    case 0:
        delta1 = -step;
        break;
    case 1:
        delta0 = -step;
        break;
    case 2:
        delta1 = step;
        break;
    case 3:
        delta0 = step;
        break;
    }
    record->position0 = (u16) record->position0 + delta0;
    record->position1 = (u16) record->position1 + delta1;
}
