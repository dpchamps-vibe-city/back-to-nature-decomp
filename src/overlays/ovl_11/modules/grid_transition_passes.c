/*
 * Whole-main-grid transition profiles selected by the phase-5 mode switch
 * in 800DDDFC. Both visit all 25x45 entries in row-major order, preserving
 * the state-dependent sequence of calls into the shared RNG. The profiles
 * differ in bounds and in which mutations precede the kind-3F transition.
 * Public contract: ovl11_grid_transition_passes.h. Grouping and boundaries:
 * notes/file-groupings.md.
 */

#include "common.h"
#include "psyq/stddef.h"
#include "ovl11_grid_entry_transitions.h"
#include "ovl11_grid_transition_passes.h"

void Ovl11PassGridKind3FTransitions(s32 mode) {
    s32 randomBound;
    s32 row;
    s32 columnsLeft;
    s32 nextRow;
    u8 *base;
    u16 *entry;

    randomBound = 0x10;
    if (mode == 0) {
        randomBound = 0x20;
    }
    row = 0;
    base = (u8 *) gMainAreaGrid;
    do {
        nextRow = row + 1;
        entry = (u16 *) (base + row * 0x168);
        for (columnsLeft = 0x2C; columnsLeft >= 0; columnsLeft--) {
            Ovl11TryGridKind3FTransition(entry, randomBound);
            entry += 4;
        }
        row = nextRow;
    } while (row < 0x19);
}

void Ovl11PassGridEntryResets(void) {
    s32 row;
    s32 column;

    for (row = 0; row < 0x19; row++) {
        for (column = 0; column < 0x2D; column++) {
            Ovl11TryResetEligibleGridEntry((Ovl11GridResetEntry *) &gMainAreaGrid[row][column], 0x10);
            Ovl11TryRefreshActiveGridEntry((Ovl11GridResetEntry *) &gMainAreaGrid[row][column], 8);
            Ovl11TryGridKind3FTransition((u16 *) &gMainAreaGrid[row][column], 0x10);
        }
    }
}
