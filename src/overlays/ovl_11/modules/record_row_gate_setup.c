/*
 * record_row_gate_setup — reconstructed translation unit.
 *
 * Apply dispatch-record row values and gates, with the season-zero
 * record-state publication/reset cases.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_record_pool_setup.h"
#include "ovl11_pool_record_transition.h"
#include "ovl11_record_row_values.h"
#include "ovl11_dispatch_record_flags.h"

void ovl_11_func_8010941C(s32 arg0);

void Ovl11SetupRecordRowsAndGates(s16 row) {
    s16 state;

    Ovl11UpdateList10RecordGates(row);
    Ovl11AssignPoolRowValues(row);
    if (((Ovl11RecordRowGateWork *)&D_8006C838)->season == 0) {
        state = *((Ovl11RecordRowGateWork *)&D_8006C838)->currentRecordState;
        if ((state == 0xD) || ((u32)(state - 0x38) < 0xBU)) {
            Ovl11PublishCurrentPoolRecord();
        }
        state = ((Ovl11RecordRowGateWork *)&D_8006C838)->mode1Selector;
        if ((state == 0xD) || ((u32)(state - 0x38) < 0xBU)) {
            ovl_11_func_8010941C(0);
        }
    }
}
