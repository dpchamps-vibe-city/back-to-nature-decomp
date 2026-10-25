/*
 * Two-record vertex-blend controller, driven by game flags 9 and 10.
 * Record flag 0x80 selects this sequence; the low nibble selects its slot.
 * Public timing/validity contract: ovl11_scene_animation_pair.h.
 * Membership and boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_scene_animation_pair.h"

s32 func_8001AF44(u32 flagIndex);
void func_8001AF70(u16 flagIndex, u16 value);

void Ovl11InitSceneAnimationPair(void) {
    s16 duration;
    s32 selector;
    s32 slot;
    s32 index;
    u16 flags;
    Ovl11SceneBlendRecordView *record;
    Ovl11SceneBlendStateView *initialBlend;

    record = (Ovl11SceneBlendRecordView *)gOvl11SceneRecords;
    gOvl11SceneAnimationPairState = 0;
    gOvl11SceneAnimationPair[0] = 0;
    gOvl11SceneAnimationPair[1] = 0;
    for (index = 0; index < 15; index++, record++) {
        flags = record->flags;
        if (flags & 0x80) {
            selector = flags & 0xF;
            switch (selector) {
            case 0:
                duration = 8;
                record->flags = flags & 0xEFFF;
                slot = 0;
                break;
            case 1:
                record->flags = flags | 0x1000;
                duration = 12;
                slot = 1;
                break;
            default:
                slot = 0;
                duration = 0;
                break;
            }
            record->blend->progress = 0;
            record->blend->duration = duration;
            gOvl11SceneAnimationPair[slot] = (u8 *)record;
        }
    }
    func_8001AF70(9, 0);
    if (func_8001AF44(10) == 1) {
        gOvl11SceneAnimationPairState = 3;
        ((Ovl11SceneBlendRecordView *)gOvl11SceneAnimationPair[0])->flags |= 0x1000;
        ((Ovl11SceneBlendRecordView *)gOvl11SceneAnimationPair[1])->flags &= 0xEFFF;
        initialBlend = ((Ovl11SceneBlendRecordView *)gOvl11SceneAnimationPair[1])->blend;
        initialBlend->progress = initialBlend->duration;
    }
}

void Ovl11StepSceneAnimationPair(void) {
    u32 phase;
    s32 flag;
    Ovl11SceneBlendRecordView *record0;
    Ovl11SceneBlendRecordView *record1;
    Ovl11SceneBlendStateView *blend0;
    Ovl11SceneBlendStateView *blend1;
    Ovl11SceneBlendStateView *resetBlend1;
    Ovl11SceneBlendStateView *resetBlend0;
    Ovl11SceneBlendStateView *switchBlend1;
    Ovl11SceneBlendStateView *switchBlend0;

    if (gOvl11SceneAnimationPair[0] != 0) {
        phase = (u32)gOvl11SceneAnimationPairState;
        switch (phase) {
        case 0:
            flag = func_8001AF44(9U);
            if (flag == 1) {
                gOvl11SceneAnimationPairState = flag;
                return;
            }
            break;
        case 1:
            record0 = (Ovl11SceneBlendRecordView *)gOvl11SceneAnimationPair[0];
            blend0 = record0->blend;
            record1 = (Ovl11SceneBlendRecordView *)gOvl11SceneAnimationPair[1];
            if ((s16)blend0->progress >= (s16)blend0->duration) {
                if (func_8001AF44(9U) == phase) {
                    switchBlend1 = record1->blend;
                    switchBlend0 = record0->blend;
                    switchBlend1->progress = 0;
                    switchBlend0->progress = 0;
                    record0->flags |= 0x1000;
                    record1->flags &= 0xEFFF;
                    func_8001AF70(9U, 0U);
                }
                if (func_8001AF44(10U) == phase) {
                    gOvl11SceneAnimationPairState = 2;
                    return;
                }
            } else {
                blend0->progress = blend0->progress + 1;
                return;
            }
            break;
        case 2:
            record0 = (Ovl11SceneBlendRecordView *)gOvl11SceneAnimationPair[1];
            record0->blend->progress = record0->blend->progress + 1;
            blend1 = record0->blend;
            if ((s16)blend1->progress >= (s16)blend1->duration) {
                blend1->progress = blend1->duration;
                gOvl11SceneAnimationPairState = 3;
                return;
            }
            break;
        case 3:
            if (func_8001AF44(10U) == 0) {
                record0 = (Ovl11SceneBlendRecordView *)gOvl11SceneAnimationPair[0];
                record1 = (Ovl11SceneBlendRecordView *)gOvl11SceneAnimationPair[1];
                record0->flags &= 0xEFFF;
                record1->flags |= 0x1000;
                resetBlend1 = record1->blend;
                resetBlend0 = record0->blend;
                resetBlend1->progress = 0;
                resetBlend0->progress = 0;
                gOvl11SceneAnimationPairState = 0;
            }
            break;
        }
    }
}
