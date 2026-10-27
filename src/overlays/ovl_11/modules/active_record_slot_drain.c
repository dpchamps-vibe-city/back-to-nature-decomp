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


void ovl_11_func_800E3C04(void);
void ovl_11_func_800F1038(u16 code, s32 value);
s32 ovl_11_func_800F0F50(void);

void Ovl11DrainActiveRecordSlot(void) {
    if (GetGameFlag(2U) == 1) {
        ovl_11_func_800E3C04();
        if (gRecordSlotPhase == 2) {
            Ovl11ServiceActiveRecordSlot();
        }
    }
    ovl_11_func_800F1038(0x12DU, 0);
    ovl_11_func_800F0F50();
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
