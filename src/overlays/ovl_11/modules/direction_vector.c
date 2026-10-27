/* Three-word movement vectors: construct a component displacement, then
 * accumulate it into a position, optionally consuming the displacement.
 * The shared vector view preserves callers' signed word layout and units.
 * Membership/boundary evidence: notes/file-groupings.md. */
#include "ovl11_direction_vector.h"

void Ovl11AccumulateDirectionVector(Recon800D0408A1View *position,
                       Recon800D0408A1View *increment, s32 retainIncrement) {
    position->unk0 = position->unk0 + increment->unk0;
    position->unk4 = position->unk4 + increment->unk4;
    position->unk8 = position->unk8 + increment->unk8;
    if (retainIncrement == 0) {
        increment->unk0 = 0;
        increment->unk4 = 0;
        increment->unk8 = 0;
    }
}

/* Select using the signed low halfword. Callers pass both lh and lhu
 * values and none consumes a return value: the output is the vector. */
void Ovl11SetDirectionVector(s32 direction, Recon800D0408A1View *vector, s32 magnitude) {
    s16 selector;

    selector = direction;
    vector->unk0 = 0;
    vector->unk4 = 0;
    vector->unk8 = 0;

    switch (selector) {
        case 0:
            vector->unk8 = -magnitude;
            return;
        case 1:
            vector->unk0 = -magnitude;
            return;
        case 2:
            vector->unk8 = magnitude;
            break;
        case 3:
            vector->unk0 = magnitude;
            break;
        case 4:
            vector->unk0 = 0;
            vector->unk4 = 0;
            vector->unk8 = 0;
            break;
        case -1:
            vector->unk4 = -magnitude;
            return;
        case -2:
            vector->unk4 = magnitude;
            break;
    }
}
