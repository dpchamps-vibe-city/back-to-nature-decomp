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

void ovl_11_func_800D7B24(void);
void ovl_11_func_800D0FB4(void);
void ovl_11_func_800F4CEC(void);
void ovl_11_func_800F4CAC(void);

void Ovl11TryClearAvailabilityEvent(void) {
    char *state;

    state = (char *)D_8006C838;
    if (*(u16 *)(state + 0x44D4) != 0 && Rand(100) < 31) {
        *(u16 *)(state + 0x44D4) = 0;
        ovl_11_func_800D7B24();
        ovl_11_func_800D0FB4();
        ovl_11_func_800F4CEC();
        ovl_11_func_800F4CAC();
        *(s32 *)(state + 0x44F8) &= ~0x2000;
        SetGameFlag(0x39, 0);
        SetGameFlag(0x3E, 0);
    }
}
