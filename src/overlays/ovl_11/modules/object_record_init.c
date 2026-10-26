/*
 * object_record_init — reconstructed translation unit.
 *
 * Claim an untagged 0xB0-byte object record and refresh its tag-dependent
 * sprite source.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_object_record_init.h"
#include "psyq/memory.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_object_id_state.h"

s32 Ovl11SetObjectRecordTag(Ovl11ObjectTagView *record, s32 tag);

s32 Ovl11InitObjectRecord(Ovl11ObjectInitRecord *record, s32 tag) {
    s16 narrowedTag;

    narrowedTag = tag;
    if (record->tag != 0) {
        return -1;
    }
    memset(record, 0, 0xB0);
    record->tag = narrowedTag;
    record->marker = 0xFFFF;
    /* Retagging deliberately repeats the tag store before sprite setup. */
    Ovl11SetObjectRecordTag((Ovl11ObjectTagView *)record, narrowedTag);
    Ovl11ResetTimedSpriteEffect(&record->timedEffect);
    return 0;
}

s32 Ovl11SetObjectRecordTag(Ovl11ObjectTagView *record, s32 tag) {
    record->tag = tag;
    Ovl11RefreshObjectSpriteForId((u16 *)record);
    return 0;
}
