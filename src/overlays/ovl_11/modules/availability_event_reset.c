/*
 * availability_event_reset — reconstructed translation unit.
 *
 * Probabilistic clearing of the shared availability selector with grid/record
 * cleanup and game-flag reset; invoked by the scene transition driver.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "random.h"
#include "game_flags.h"
#include "ovl11_availability_event_reset.h"
#include "ovl11_pool_record_transition.h"
#include "ovl11_grid_growth.h"
#include "ovl11_record_position_recovery.h"

void Ovl11TryClearAvailabilityEvent(void) {
    char *state;

    state = (char *)D_8006C838;
    if (*(u16 *)(state + 0x44D4) != 0 && Rand(100) < 31) {
        *(u16 *)(state + 0x44D4) = 0;
        Ovl11ResetAlternateAreaGrid();
        Ovl11RecoverRecordPositions();
        Ovl11PublishCurrentPoolRecord();
        Ovl11PublishNextPoolRecord();
        *(s32 *)(state + 0x44F8) &= ~0x2000;
        SetGameFlag(0x39, 0);
        SetGameFlag(0x3E, 0);
    }
}
