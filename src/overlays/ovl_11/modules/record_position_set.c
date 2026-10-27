/*
 * record_position_set — reconstructed translation unit.
 *
 * Assign three signed position words at record offsets 0x58/0x5C/0x60 and
 * mark the new position in its flags. The four-word by-value input retains
 * the shared position ABI and its ignored fourth word.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_record_position_set.h"

void Ovl11SetRecordPosition(Ovl11SetFieldsView *record, Ovl11PaddedVec3 position) {
    record->field_58 = position.vx;
    record->field_5C = position.vy;
    record->field_60 = position.vz;
    record->field_34 = (record->field_34 | 0x2000) & ~0x4000;
}
