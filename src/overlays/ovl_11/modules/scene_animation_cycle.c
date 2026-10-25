/*
 * Three-record vertex-blend cycle, started by game flag 7.
 * Scene setup indexes the records; updates complete all three blends before
 * acknowledging the trigger. Public contract: ovl11_scene_animation_cycle.h.
 * Membership/boundary evidence: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_scene_animation_cycle.h"

void Ovl11InitSceneAnimationCycle(void) {
    s16 duration;
    s32 selector;
    s32 slot;
    s32 index;
    u16 flags;
    u8 *record;

    record = gOvl11SceneRecords;
    gOvl11SceneAnimationCycle[0] = 0;
    gOvl11SceneAnimationCycle[1] = 0;
    gOvl11SceneAnimationCycle[2] = 0;
    gOvl11SceneAnimationCycleState = 0;
    for (index = 0; index < 15; index++, record += 0x30) {
        flags = *(u16 *)(record + 0x28);
        if (flags & 0x100) {
            selector = flags & 0xF;
            switch (selector) {
            case 0:
                *(u16 *)(record + 0x28) = flags & 0xEFFF;
                duration = 8;
                slot = 0;
                break;
            case 1:
                *(u16 *)(record + 0x28) = flags | 0x1000;
                duration = 4;
                slot = 1;
                break;
            case 2:
                *(u16 *)(record + 0x28) = flags | 0x1000;
                duration = 12;
                slot = 2;
                break;
            default:
                duration = 0;
                slot = 0;
                break;
            }
            *(s16 *)(*(u8 **)(record + 0x2C) + 0x2E) = 0;
            *(s16 *)(*(u8 **)(record + 0x2C) + 0x30) = duration;
            gOvl11SceneAnimationCycle[slot] = record;
        }
    }
}

/* The increment wraps as u16, but the endpoint comparison is signed. Keep
 * these two witnessed views distinct instead of silently changing either. */
typedef struct {
    char pad[0x2E];
    u16 progress;
} CycleUnsignedProgress;

typedef struct {
    char pad[0x2E];
    s16 progress;
    s16 duration;
} CycleSignedProgress;

typedef struct {
    char pad[0x28];
    u16 flags;
    char pad2[2];
    void *blend;
} CycleRecord;

s32 func_8001AF44(u32 flagIndex);
void func_8001AF70(u16 flagIndex, u16 value);

void Ovl11StepSceneAnimationCycle(void) {
    /* This is a real nested helper: the original calls pass a static chain.
     * Keeping it nested preserves that ABI without register-capture asm. */
    s32 Ovl11AdvanceSceneAnimationCycle(CycleRecord *from, CycleRecord *to, s32 nextPhase) {
        CycleUnsignedProgress *increment;
        CycleSignedProgress *compare;

        increment = from->blend;
        increment->progress = increment->progress + 1;
        compare = from->blend;
        if (compare->progress < compare->duration) {
            return 0;
        }
        *(s16 *)((u8 *)to->blend + 0x2E) = 0;
        compare->progress = 0;
        from->flags |= 0x1000;
        to->flags &= 0xEFFF;
        gOvl11SceneAnimationCycleState = nextPhase;
        return 1;
    }
    CycleRecord *first;
    s32 one;

    first = (CycleRecord *)gOvl11SceneAnimationCycle[0];
    if (first != 0) {
        one = 1;
        if (gOvl11SceneAnimationCycleState == one) goto stage_1;
        if (gOvl11SceneAnimationCycleState == 0) goto idle;
        if (gOvl11SceneAnimationCycleState == 2) goto stage_2;
        if (gOvl11SceneAnimationCycleState == 3) goto stage_3;
    }
    return;
idle:
    if (func_8001AF44(7) == one) {
        gOvl11SceneAnimationCycleState = one;
    }
    return;
stage_1:
    Ovl11AdvanceSceneAnimationCycle(first, (CycleRecord *)gOvl11SceneAnimationCycle[1], 2);
    return;
stage_2:
    Ovl11AdvanceSceneAnimationCycle((CycleRecord *)gOvl11SceneAnimationCycle[1],
                                  (CycleRecord *)gOvl11SceneAnimationCycle[2], 3);
    return;
stage_3:
    if (Ovl11AdvanceSceneAnimationCycle((CycleRecord *)gOvl11SceneAnimationCycle[2], first, 0) == one) {
        func_8001AF70(7, 0);
    }
}
