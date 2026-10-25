/*
 * Embedded record-sprite animation selection. Avoid reselecting the current
 * animation, which would reset its frame and timer. The bulk entry points
 * apply the same operation to all six 0x50-byte records.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "sprite_source.h"
#include "ovl17_record_sprite_animation.h"

/* Preserve the legacy return arm and fall-through call: the callers discard
 * the result, and the changed-animation path has no defined return value. */
s32 Ovl17SelectRecordAnimation2(Ovl17RecordAnimationView *record) {
    if (record->animationIndex == 2) {
        return 2;
    } else {
        SelectSpriteAnimation((ObjectState *)((u8 *)record + 0x20), 2);
    }
}

void Ovl17SelectAllRecordAnimations2(void) {
    u8 *record;
    s32 remaining;

    record = gOvl17AnimatedRecordBase;
    for (remaining = 5; remaining >= 0; remaining--) {
        Ovl17SelectRecordAnimation2((Ovl17RecordAnimationView *)record);
        record += 0x50;
    }
}

s32 Ovl17SelectRecordAnimation0(Ovl17RecordAnimationView *record) {
    if (record->animationIndex == 0) {
        return record->animationIndex;
    } else {
        SelectSpriteAnimation((ObjectState *)((u8 *)record + 0x20), 0);
    }
}

void Ovl17SelectAllRecordAnimations0(void) {
    u8 *record;
    s32 remaining;

    record = gOvl17AnimatedRecordBase;
    for (remaining = 5; remaining >= 0; remaining--) {
        Ovl17SelectRecordAnimation0((Ovl17RecordAnimationView *)record);
        record += 0x50;
    }
}

s32 Ovl17SelectRecordAnimation1(Ovl17RecordAnimationView *record) {
    if (record->animationIndex == 1) {
        return 1;
    } else {
        SelectSpriteAnimation((ObjectState *)((u8 *)record + 0x20), 1);
    }
}
