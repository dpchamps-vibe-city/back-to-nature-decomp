/* Fixed animation/hold playlists and their callback completion protocol.
 * The eight private tables occur in callback order in original overlay data.
 * Keep separate callbacks: the dispatch table and lock/timer differences are
 * part of the interface, not duplicate code to collapse. See the public
 * header and notes/file-groupings.md for behavior and boundary evidence. */
#include "ovl11_animation_sequences.h"
#include "ovl11_keyframe_step.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_record_movement.h"

s32 Ovl11RunAnimation0Sequence(Ovl11D04D4Obj *object) {
    s16 state;
    s32 complete;

    state = object->unk28;
    complete = 0;
    switch (state) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        complete = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11Animation0Sequence, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return complete;
}

s32 Ovl11RunAnimation052Sequence(Ovl11D04D4Obj *object) {
    s16 state;
    s32 complete;

    state = object->unk28;
    complete = 0;
    switch (state) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        complete = 1;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11Animation052Sequence, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return complete;
}

s32 Ovl11RunAnimation50Sequence(Ovl11D04D4Obj *object) {
    s16 state;
    s32 complete;

    state = object->unk28;
    complete = 0;
    switch (state) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case 4:
        complete = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11Animation50Sequence, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = 4;
        }
        break;
    }
    return complete;
}

s32 Ovl11RunAnimation41Sequence(Ovl11D04D4Obj *object) {
    s32 complete;

    complete = 0;
    switch (object->unk28) {
    case 0:
        object->unk28 = 1;
        object->unk2C = 0;
        object->unk34 |= 0x800;
        Ovl11StepRecordMovement((Recon_ovl_11_func_800D2594_A0View *)object, 0x12, 0x8031);
        break;
    case -1:
        complete = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x4000;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (object->unk2C != 0) {
            object->unk2C--;
        }
        if (Ovl11StepPackedKeyframes(object, gOvl11Animation41Sequence, &object->unk28, -1, (u16 *)&object->unk2C) != 0) {
            object->unk28 = -1;
        } else {
            Ovl11StepRecordMovement((Recon_ovl_11_func_800D2594_A0View *)object, 0x12, 0x8031);
        }
        break;
    }
    return complete;
}

s32 Ovl11RunAnimation70Sequence(Ovl11D04D4Obj *object) {
    s16 state;
    s32 complete;

    state = object->unk28;
    complete = 0;
    switch (state) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        complete = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11Animation70Sequence, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return complete;
}

s32 Ovl11RunAnimation60Sequence(Ovl11D04D4Obj *object) {
    s16 state;
    s32 complete;

    state = object->unk28;
    complete = 0;
    switch (state) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        complete = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11Animation60Sequence, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return complete;
}

s32 Ovl11RunAnimation3Sequence(Ovl11D04D4Obj *object) {
    s16 state;
    s32 complete;

    state = object->unk28;
    complete = 0;
    switch (state) {
    case 0:
        object->unk28 = 1;
        break;
    case -1:
        complete = 1;
        object->unk34 = object->unk34 & ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11Animation3Sequence, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return complete;
}

s32 Ovl11RunAnimation8Sequence(Ovl11D04D4Obj *object) {
    s16 state;
    s32 complete;

    state = object->unk28;
    complete = 0;
    switch (state) {
    case 0:
        object->unk28 = 1;
        break;
    case -1:
        complete = 1;
        object->unk34 = object->unk34 & ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11Animation8Sequence, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return complete;
}
