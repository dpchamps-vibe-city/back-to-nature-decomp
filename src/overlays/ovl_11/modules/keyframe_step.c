/*
 * Packed-keyframe playback shared by state handlers throughout ovl_11.
 * Caller-owned indices/ticks select four-byte code/duration rows. The
 * six-byte window below deliberately overlaps the next row's code.
 * See ovl11_keyframe_step.h for mode gating, wrapping and termination.
 * Membership and evidence-limited boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_keyframe_step.h"

typedef struct {
    s16 code;
    s16 duration;
    s16 nextCode;
} Ovl11PackedKeyframeWindow;

s32 Ovl11StepPackedKeyframes(Ovl11D04D4Obj *object, u8 *table, s16 *index, s32 bias, u16 *ticks) {
    Ovl11PackedKeyframeWindow *row;
    char *farWork;
    s16 code;
    s32 finished;
    s32 nextTick;

    row = (Ovl11PackedKeyframeWindow *)(table + (*index << 2) + ((s16)bias << 2));
    farWork = (char *)&D_8007AFF0;
    code = row->code;
    object->unk24 = code;
    nextTick = *ticks + 1;
    *ticks = nextTick;
    finished = 0;
    if (object->unk30 == *(s16 *)(farWork + 0x25476)) {
        if ((object->unk7A & 0x300) && (row->duration <= (s16)nextTick)) {
            *index = *index + 1;
            *ticks = 0;
            code = row->nextCode;
            finished = (code == -1);
        }
    } else if (row->duration <= (s16)nextTick) {
        *index = *index + 1;
        *ticks = 0;
        code = row->nextCode;
        if (code == -1) {
            finished = 1;
        }
    }
    if (finished == 0) {
        object->unk24 = code;
    }
    return finished;
}
