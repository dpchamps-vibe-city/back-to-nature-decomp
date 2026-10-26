/* Actor/anchor-record control-prefix writers. Both store state, direction
 * and a caller-defined timer/control value, but remain distinct external
 * entries for their different client families. No private linkage is inferred.
 * Membership/boundaries: notes/file-groupings.md. */

#include "ovl23_record_control_fields.h"

void Ovl23SetActorControl(Ovl23Func800BB0C8Arg *record,
                          s32 state, s32 direction, s32 controlValue) {
    record->unk4 = state;
    record->unk6 = direction;
    record->unk8 = controlValue;
}

void Ovl23SetAnchorRecordControl(Ovl23Func800BB0C8Arg *record,
                                 s32 state, s32 direction, s32 controlValue) {
    record->unk4 = state;
    record->unk6 = direction;
    record->unk8 = controlValue;
}
