/*
 * object_event_slot_flags_command — reconstructed translation unit.
 *
 * Clear selected event-slot status bits on one record or broadcast the same
 * operation to slots one through thirty-six.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_object_event_slot_flags_command.h"

s32 Ovl11ClearObjectEventSlotFlags(s16 slot, s16 mode, s32 unused2, s32 unused3) {
    s32 i;
    struct_80076220 *record;
    u16 flags;

    if (slot == 0x2A) {
        for (i = 1; i < 0x25; i++) {
            Ovl11ClearObjectEventSlotFlags((s16) i, mode, unused2, unused3);
        }
        return 1;
    }
    /* The original upper-bound test is on mode, not slot. */
    if ((slot > 0) && (mode < 0x25)) {
        record = &gObjectEventSlotBase + slot;
        if (mode != 2) {
            record->unk1E &= 0xE7FF;
        }
        if (mode != 0) {
            flags = record->unk1E;
            if (flags & 0x100) {
                record->unk1E = flags & 0xFEFF;
            }
        }
    } else {
        return 0;
    }
    return 1;
}
