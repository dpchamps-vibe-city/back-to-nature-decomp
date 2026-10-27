/*
 * pair_slot_record_update — reconstructed translation unit.
 *
 * Update one of two dispatch records from pair-slot enable and variant
 * inputs; storage and slot state remain owned by their existing services.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_pair_slot_records.h"

/* Signed kind view of the same 0x18-byte dispatch-record storage. */
typedef struct {
    char pad0[0x2];
    s16 kind;
    u16 flags;
} Ovl11PairSlotRecordView;

void Ovl11UpdatePairSlotRecord(s32 index, s32 enabled, s16 alternateKind) {
    s32 *work;
    s32 *upperWork;
    Ovl11PairSlotRecordView *record;
    u16 flags;

    work = (s32 *)&D_8006C838;
    upperWork = work + (0x8000 >> 2);
    /* Slot 3's records 10/11 correspond to pair slots 2/3. Keep the
     * two-stage work address and the original signed-halfword kind view. */
    record = (Ovl11PairSlotRecordView *)(*(char **)((char *)upperWork + 0x5D98) + ((index * 0x18) + 0xF0));

    if (enabled == 1) {
        flags = record->flags | 1;
    } else {
        flags = record->flags & 0xFFFE;
    }
    record->flags = flags;

    if (alternateKind == 0) {
        record->kind = 0x123;
    } else {
        record->kind = 0xE8;
    }
}
