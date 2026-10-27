/* Ordered pair-table installation, ovl_11:800F0C70.
 * Public contract: ovl11_record_remap_tables.h.
 * Membership/boundaries: notes/file-groupings.md. */
#include "common.h"
#include "game_types.h"
#include "ovl11_record_remap_tables.h"

void Ovl11ApplyRecordRemapTables(void) {
    u32 i;
    Ovl11RecordPairAssignment *assignment;
    struct_80076220 *record;
    s16 index;
    struct_80076220 *eventRecords;
    s16 sentinel;

    for (i = 0; i < 7U; i++) {
        char *parallelRecord = (char *)D_8006C838 + i * 0xF8;
        *(u16 *)(parallelRecord + 0x8000 + 0x5EC4) = gOvl11ParallelRecordPairDefaults[i].first;
        *(u16 *)(parallelRecord + 0x8000 + 0x5EC6) = gOvl11ParallelRecordPairDefaults[i].second - 1;
    }
    for (i = 0; i < 5U; i++) {
        record = &gObjectEventSlotBase + gOvl11RecordPairDefaults[i].recordIndex;
        record->unk24 = gOvl11RecordPairDefaults[i].first;
        record->unk26 = gOvl11RecordPairDefaults[i].second;
    }
    for (i = 0; i < 5U; i++) {
        assignment = gOvl11ConditionalRecordPairDefaults + i;
        record = &gObjectEventSlotBase + assignment->recordIndex;
        if (((Ovl11SpecialRecordPairView *)D_8006C838)->first == record->unk24 &&
            ((Ovl11SpecialRecordPairView *)D_8006C838)->second == record->unk26) {
            record->unk24 = assignment->first;
            record->unk26 = assignment->second;
        }
    }
    sentinel = -1;
    eventRecords = &gObjectEventSlotBase;
    assignment = gOvl11RecordPairOverrideList;
applyOverride:
    index = assignment->recordIndex;
    if (index != sentinel) {
        record = (struct_80076220 *)(index * 0x1D4 + (u32)eventRecords);
        record->unk24 = assignment->first;
        record->unk26 = assignment->second;
        assignment++;
        goto applyOverride;
    }
}
