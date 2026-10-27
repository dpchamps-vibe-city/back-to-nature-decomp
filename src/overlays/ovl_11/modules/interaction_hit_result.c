/*
 * Snapshot and consume the interaction hit latch, resolving the selected
 * category into an item result. The seasonal path mutates its source record;
 * the other supported paths resolve/consume a cell record. Unsupported flag
 * combinations still consume the latch but leave the output untouched.
 * See ovl11_interaction_hit_result.h and notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "sound_commands.h"
#include "ovl11_cell_actions.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_seasonal_record_ids.h"
#include "ovl11_interaction_hit_result.h"
#include "ovl11_interaction_record_prepare.h"

s32 Ovl11ResolveInteractionHitResult(Ovl11InteractionHitObject *object,
                                   Ovl11RecordPresentation *result) {
    s32 itemFlag;
    u32 category;

    object->hitRecord = gOvl11InteractionHitRecordView;
    object->hitFlags = gOvl11InteractionHitFlags;
    category = gOvl11InteractionHitFlags & 0x8928;
    /* This is an exact category match, not a priority test of individual bits. */
    switch (category) {
    case 0x8000:
        itemFlag = Ovl11ItemHasFlag8100(Ovl11GetSeasonalRecordId((Ovl11SeasonalRecordView *)gOvl11InteractionHitRecordView));
        if ((itemFlag == 1) && (Ovl11ConsumeSeasonalRecord((Ovl11SeasonalRecordView *)gOvl11InteractionHitRecordView, result) == itemFlag)) {
            PlaySoundEffect(7);
        }
        break;
    case 0x100:
        PlaySoundEffect(7);
        if (Ovl11PrepareInteractionRecord((Ovl11SeasonalRecordView *)object->hitRecord) != 2) {
            result->id = object->hitRecord->u0.unk0;
        } else {
            result->id = 0x65;
        }
        (*(s16 *)((u8 *)result + 2)) = 0;
        result->valid = 1;
        break;
    case 0x800:
    case 0x8:
        PlaySoundEffect(7);
        result->id = object->hitRecord->u0.unk0;
        (*(s16 *)((u8 *)result + 2)) = 0;
        result->valid = 1;
        Ovl11PrepareInteractionRecord((Ovl11SeasonalRecordView *)object->hitRecord);
        break;
    case 0x20:
        PlaySoundEffect(7);
        result->id = Ovl11ResolveCellItemState(object->hitRecord);
        (*(s16 *)((u8 *)result + 2)) = 0;
        result->valid = 1;
        break;
    }
    gOvl11InteractionHitFlags = 0;
    gOvl11InteractionHitRecordView = NULL;
    return 0;
}
