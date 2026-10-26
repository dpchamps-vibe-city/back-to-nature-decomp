/*
 * Advance a decision record along the track in signed Q12 coordinates.
 * State 2/3 selects one of two speeds; direction 6 moves toward the lower end.
 * The endpoint tests are deliberately independent, and record 2 keeps its
 * control fields when clamped. See ovl23_record_decision_advance.h.
 */
#include "common.h"
#include "game_types.h"
#include "ovl23_record_control_fields.h"
#include "ovl23_record_decision_advance.h"

void Ovl23AdvanceDecisionRecordPosition(Ovl23Func800BB0C8Arg *record) {
    s32 step12;
    s32 upperEndpoint12;

    switch (record->unk4) {
    case 0:
        return;
    case 2:
        step12 = (Ovl23_D_800BF87C_bounds.unk3A0 << 0xC) / 10;
        break;
    case 3:
        step12 = (Ovl23_D_800BF87C_bounds.unk3A2 << 0xC) / 10;
        break;
    default:
        return;
    }

    if (record->unk6 == 6) {
        *(s32 *)((u8 *)record + 0xC) -= step12;
    } else {
        *(s32 *)((u8 *)record + 0xC) += step12;
    }
    upperEndpoint12 = (Ovl23_D_800BF87C_bounds.unk39C + 0x77) << 0xC;
    if (*(s32 *)((u8 *)record + 0xC) >= upperEndpoint12) {
        *(s32 *)((u8 *)record + 0xC) = upperEndpoint12;
        if (*(s16 *)((u8 *)record + 0) != 2) {
            Ovl23SetAnchorRecordControl(record, 1, 6, record->unk8);
        }
    }
    if (*(s32 *)((u8 *)record + 0xC) <= -0x30000) {
        *(s32 *)((u8 *)record + 0xC) = -0x30000;
        if (*(s16 *)((u8 *)record + 0) != 2) {
            Ovl23SetAnchorRecordControl(record, 1, 2, record->unk8);
        }
    }
}
