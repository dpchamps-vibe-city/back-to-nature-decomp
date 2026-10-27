/*
 * Jittered arc/tangent position command, ovl_11:800EB328 (opcode entry).
 * Resolve field 22 as a word XYZ vector. Value slots supply arcStep,
 * tangentStep and consecutive X/Z origins. Angles use steps of pi/24.
 * arcStep < 0: set Y=-700 and add 157 to X, leaving Z unchanged.
 * tangentStep >= arcStep: set XYZ on a radius-1200 arc about the origins,
 * with independent 0..50 jitter minus 25 on X/Z; Y=max(random-700, oldY-25).
 * Otherwise add a radius-157 tangent step to X/Z, set Y=-700 and return 1.
 * The other branches return 0. Always select facing animation 7/8/9/10
 * from the dominant truncated X/Z displacement (ties favor X).
 * Three random draws occur in EVERY branch. Units are unestablished; slot
 * indices, record resolution and arithmetic/conversion ranges are unchecked.
 * No C callers. Membership/boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "psyq/libmath.h"
#include "psyq/abs.h"
#include "random.h"
#include "ovl11_state_field_access.h"
#include "ovl11_state_record_commands.h"

s32 Ovl11UpdateRandomizedArcPosition(s16 recordId, s16 arcStepSlot,
                                    s32 tangentStepSlot, s32 originSlot) {
    s32 *position;
    s32 x;
    s32 y;
    s32 z;
    s32 random;
    s32 xOrigin;
    s32 zOrigin;
    s32 slot;
    double dx;
    double dz;
    s32 tangentTaken;
    s32 animation;

    slot = originSlot;
    position = (s32 *)Ovl11ResolveStateField(0x22, recordId, 0);
    random = Rand(51);
    xOrigin = gOvl11ValueSlots[slot] - 25;
    x = random + xOrigin;
    y = (s32)Rand(51) - 700;
    random = Rand(51);
    zOrigin = gOvl11ValueSlots[++slot] - 25;
    z = random + zOrigin;
    tangentTaken = 0;
    if (y < position[1] - 25) {
        y = position[1] - 25;
    }
    dz = 3.14159 / 24;
    if (gOvl11ValueSlots[arcStepSlot] < 0) {
        dx = 157.0795;
        dz = 0.0;
        position[1] = -700;
        position[0] += 157;
    } else if (gOvl11ValueSlots[tangentStepSlot] >= gOvl11ValueSlots[arcStepSlot]) {
        register double radius = 1200.0;

        dz *= gOvl11ValueSlots[arcStepSlot];
        dx = radius * sin(dz);
        dz = radius * cos(dz);
        position[1] = y;
        position[0] = x + dx;
        position[2] = z + dz;
    } else {
        register double radius = 157.0;

        dz *= gOvl11ValueSlots[tangentStepSlot];
        dx = radius * sin(dz + 1.570795);
        dz = radius * cos(dz + 1.570795);
        position[1] = -700;
        position[0] += (s32)dx;
        position[2] += (s32)dz;
        tangentTaken = 1;
    }
    if (abs((s32)dx) >= abs((s32)dz)) {
        if (dx < 0.0) {
            animation = 9;
        } else {
            animation = 7;
        }
    } else {
        if (dz < 0.0) {
            animation = 8;
        } else {
            animation = 10;
        }
    }
    Ovl11StateRecordAnimationCommand(recordId, animation, 0, 0);
    return tangentTaken;
}
