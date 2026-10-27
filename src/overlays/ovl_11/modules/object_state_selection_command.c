/*
 * object_state_selection_command — reconstructed translation unit.
 *
 * Set a compact record direction or change and synchronously update the
 * primary object state while preserving its work flags.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_primary_object_update.h"
#include "ovl11_object_state_selection_command.h"

s32 Ovl11SelectObjectStateOrDirectionCommand(s16 stateOrDirection, s16 recordSelector, s32 unused2, s32 unused3) {
    s32 savedFlags;
    s32 index = recordSelector;

    if (index >= 0x32) {
        u8 *base = (u8 *) D_8006C838;
        u8 *record;

        index -= 0x32;
        record = base + index * 0xF8;

        ((struct Ovl11CompactRecordDirectionView *) record)->direction = stateOrDirection;
    } else {
        if (((Ovl11CBB9CGlobals *) D_8006C838)->state != stateOrDirection) {
            savedFlags = ((struct struct_8006C838_800E4608 *) D_8006C838)->field_0C;
            ((Ovl11CBB9CGlobals *) D_8006C838)->state = stateOrDirection;
            ((struct struct_8006C838_800E4608 *) D_8006C838)->field_0C = savedFlags & 0xF7FFFFFF;
            Ovl11UpdatePrimaryObject();
            ((struct struct_8006C838_800E4608 *) D_8006C838)->field_0C = savedFlags;
            return 1;
        }
        return 1;
    }
    return 1;
}
