/* Retirement pass over the two record pools, including held-item cleanup.
 * Keep the two counter updates inline and in this order: each increment
 * narrows in memory before its upper-bound test. Factoring them into a
 * different arithmetic model would change wrap behavior.
 * Original entry: ovl_11:800CFC20; boundaries: notes/file-groupings.md. */
#include "ovl11_flagged_record_retirement.h"
#include "game_types.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_held_item_replacement.h"
#include "ovl11_small_record_handlers.h"
#include "ovl11_code160_record_handlers.h"

void Ovl11RetireFlaggedRecords(void) {
    struct Ovl11ResetB8 *largeRecord;
    struct Ovl11ResetB4 *smallRecord;
    s32 flags;
    s32 index;

    largeRecord = (struct Ovl11ResetB8 *) &gTaggedRecordBase;
    for (index = 19; index >= 0; index--) {
        if (largeRecord->flags & 0x100) {
            Ovl11ClearCode160Record((s32) largeRecord);
            if ((s16) ++((struct Ovl11RecordRetirementWork *) D_8006C838)->count >= 0x10) {
                ((struct Ovl11RecordRetirementWork *) D_8006C838)->count = 0xFU;
            }
            ((struct Ovl11RecordRetirementWork *) D_8006C838)->byteCount++;
            ((struct Ovl11RecordRetirementWork *) D_8006C838)->byteCount =
                ((struct Ovl11RecordRetirementWork *) D_8006C838)->byteCount >= 100 ?
                99 : ((struct Ovl11RecordRetirementWork *) D_8006C838)->byteCount;
        }
        largeRecord++;
    }
    smallRecord = &((struct Ovl11RecordRetirementWork *) D_8006C838)->records[0];
    for (index = 0; index < 10; index++) {
        flags = smallRecord->flags;
        if (flags & 0x100) {
            if (flags & 0x20000) {
                ((struct Ovl11RecordRetirementWork *) D_8006C838)->flags &= 0xEFFF7FFF;
                Ovl11ResetPrimaryObjectRequest();
                Ovl11ReleaseHeldItem((Ovl11C7010Object *) ((struct Ovl11RecordRetirementWork *) D_8006C838)->object);
            }
            Ovl11ClearSmallRecord((s32) smallRecord);
            if ((s16) ++((struct Ovl11RecordRetirementWork *) D_8006C838)->count >= 0x10) {
                ((struct Ovl11RecordRetirementWork *) D_8006C838)->count = 0xFU;
            }
            ((struct Ovl11RecordRetirementWork *) D_8006C838)->byteCount++;
            ((struct Ovl11RecordRetirementWork *) D_8006C838)->byteCount =
                ((struct Ovl11RecordRetirementWork *) D_8006C838)->byteCount >= 100 ?
                99 : ((struct Ovl11RecordRetirementWork *) D_8006C838)->byteCount;
        }
        smallRecord++;
    }
}
