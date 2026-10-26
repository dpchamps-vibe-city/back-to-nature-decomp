/*
 * Collision-driven current/next pool-record transitions. The two publishers
 * supply the probes' common output; the completion helper clears the exact
 * gate/record state those probes consume. No allocation or storage ownership
 * is implied by publishing a pointer. Membership: notes/file-groupings.md.
 */
#include "ovl11_pool_record_transition.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "ovl11_collision_query_budget.h"
#include "ovl11_interaction_volume_query.h"
#include "ovl11_surface_height.h"

s32 ovl_11_func_800F44B4(u16 id, s32 *position, s16 *selector, s32 mode);
void Ovl11CompletePoolGate(void);

void Ovl11PublishNextPoolRecord(void) {
    char *base;
    Ovl11PublishedPoolRecord *current;
    Ovl11PublishedPoolRecord *next;

    /* Keep the two-stage shared-work address; do not fold the 0x8000 step
     * into the field displacement. */
    base = (char *)&D_8006C838;
    base += 0x8000;
    current = *(Ovl11PublishedPoolRecord **)(base + 0x5DD4);
    next = (Ovl11PublishedPoolRecord *)((char *)current + 0x18);
    gOvl11PublishedPoolRecord = next;
    next->state = 1;
    next->x = -0x4B0;
    next->y = 0;
    next->z = 0xB54;
}

void Ovl11PublishCurrentPoolRecord(void) {
    char *base;
    Ovl11PublishedPoolRecord *current;

    base = (char *)&D_8006C838;
    base += 0x8000;
    current = *(Ovl11PublishedPoolRecord **)(base + 0x5DD4);
    gOvl11PublishedPoolRecord = current;
    current->state = 1;
    current->x = -0x1388;
    current->y = 0;
    current->z = 0x8FC;
}

void Ovl11ProbePoolRecordTransitions(void) {
    s32 position[4];
    Ovl11CollisionTag tag;
    s32 hit;
    s32 invalidSelector;

    hit = 0;
    if ((ovl_11_func_800F44B4(0x64, position, &tag.field_2, 0) == 0) &&
        (((Ovl11SpawnProbeState *) D_8006C838)->flags & 0x800) &&
        (((Ovl11SpawnProbeState *) D_8006C838)->selected == 0x64)) {
        hit = 1;
        ((Ovl11SpawnProbeState *) D_8006C838)->record[2] |= 1;
    }
    if (Ovl11QueryCollisionBudgeted(position, tag, 0, 0, NULL, NULL, 0, 0x20) != 0) {
        hit = 1;
    }
    invalidSelector = 0;
    if (hit != 0) {
        Ovl11PublishCurrentPoolRecord();
    }
    ovl_11_func_800F44B4(0x65, position, &tag.field_2, 0);
    hit = Ovl11QueryCollisionBudgeted(position, tag, 0, 0, NULL, NULL, 0, 0x20) != 0;
    if (tag.field_2 >= 7) {
        goto invalid;
    }
    if (tag.field_2 > 0) {
        goto valid;
    }
invalid:
    invalidSelector = 1;
valid:
    if (hit != 0) {
        Ovl11PublishNextPoolRecord();
    }
    if (invalidSelector != 0) {
        Ovl11CompletePoolGate();
    }
}

void Ovl11ProbeActivePoolGate(void) {
    Ovl11Func800BDCE8Arg1 *position;
    s32 hit;
    Ovl11PoolGateRecord *entry;
    s32 type;
    u8 *base;

    if (((Ovl11CollisionWorkView *)&D_8007AFF0)->collision.field_0 == 0 &&
        (((Ovl11PoolGateState *)D_8006C838)->flags & 0x02000000)) {
        entry = ((Ovl11PoolGateState *)D_8006C838)->entry;
        position = (Ovl11Func800BDCE8Arg1 *) entry;
        if (((Ovl11CollisionWorkView *)&D_8007AFF0)->collision.field_2 != (*(s16 *) ((u8 *) position + 0x18))) {
            return;
        }
        position = (Ovl11Func800BDCE8Arg1 *) ((u8 *)position + 0x20);
        hit = Ovl11ProbeInteractionVolume(((Ovl11FarInitView *)&D_8007AFF0)->data, position, 0) != NULL;
        if (Ovl11ResolveSurfaceHeight(&position->unk0, 0xC000) != 0) {
            hit = 1;
        }
        if (hit != 0) {
            Ovl11CompletePoolGate();
        }
    }
    if (((Ovl11PoolGateState *)D_8006C838)->flags & 0x800) {
        base = (u8 *)D_8006C838;
        base += 0x8000;
        type = ((Ovl11CollisionWorkView *)&D_8007AFF0)->collision.field_2;
        entry = *(Ovl11PoolGateRecord **)(base + 0x5DD4);
        if (type == entry->type) {
            position = (Ovl11Func800BDCE8Arg1 *) entry->position;
            hit = Ovl11ProbeInteractionVolume(((Ovl11FarInitView *)&D_8007AFF0)->data, position, 0) != NULL;
            if (Ovl11ResolveSurfaceHeight(&position->unk0, 0xC000) != 0) {
                hit = 1;
            }
            if (hit != 0) {
                Ovl11PublishCurrentPoolRecord();
            }
        }
    }
}

void Ovl11CompletePoolGate(void) {
    Ovl11PoolCompletionWork *work = (Ovl11PoolCompletionWork *)&D_8006C838;

    if (work->flags & 0x02000000) {
        Ovl11PoolCompletionEntry *entry = work->current;

        work->flags &= 0xFDFFFFFF;
        entry += 1;
        entry->flags &= 0xFFFE;
        work->completion = 0;
    }
}
