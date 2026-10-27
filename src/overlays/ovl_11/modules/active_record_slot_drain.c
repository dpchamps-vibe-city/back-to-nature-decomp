/*
 * active_record_slot_drain — reconstructed translation unit.
 *
 * Blocking active-record-slot service before a dependent action. Requests
 * object-program release when enabled, posts record code 0x12D, and services
 * until the program is runnable or flag 2 clears.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_active_record_slot_drain.h"
#include "game_flags.h"
#include "ovl11_record_slot_table.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libapi.h"
#include "psyq/memory.h"
#include "ovl11_object_archive_pipeline.h"
#include "ovl11_condition_evaluation.h"


void Ovl11DrainActiveRecordSlot(void) {
    if (GetGameFlag(2U) == 1) {
        Ovl11RequestObjectProgramRelease();
        if (gRecordSlotPhase == 2) {
            Ovl11ServiceActiveRecordSlot();
        }
    }
    Ovl11QueueConditionCodeAnySlot(0x12DU, 0);
    Ovl11EvaluatePendingConditions();
    Ovl11ServiceActiveRecordSlot();
    /* State 1 permits the next action; service it once more before return. */
    if (GetGameFlag(2U) == 1) {
        do {
            if (((struct Ovl11E4280State *)D_8006C838)->selector == 1) {
                Ovl11ServiceActiveRecordSlot();
                break;
            }
            Ovl11ServiceActiveRecordSlot();
        } while (GetGameFlag(2U) != 0);
    }
}
