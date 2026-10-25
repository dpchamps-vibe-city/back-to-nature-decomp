/* Overlay-15 five-slot coordinate rows; X precedes Z in original link order.
 * These are separate overlay entry points/tables from the overlay-11 twins.
 * Membership and boundary evidence: notes/file-groupings.md. */
#include "common.h"
#include "game_types.h"
#include "ovl15_row_placement.h"
#include "random.h"

void Ovl15PlaceRowXSlot(RowXPlacement *output, s16 slot) {
    s16 row = slot / 5;
    s16 column = slot % 5;
    s32 direction;

    output->x = gOvl15RowXPlacementTable[row][0] + column * 0x190;
    output->y = gOvl15RowXPlacementTable[row][1];
    output->z = gOvl15RowXPlacementTable[row][2];
    direction = Rand(4);
    output->direction = direction;
    output->mode = 4;
}

s32 Ovl15PlaceRowZSlot(RowZPlacement *output, s16 slot) {
    s16 selected = slot;
    s16 row;
    s16 column;
    s32 mode;

    if (selected == -1) {
        row = 4;
        column = 0;
    } else if (selected == -2) {
        row = 5;
        column = 0;
    } else {
        row = selected / 5;
        column = selected % 5;
    }
    output->x = gOvl15RowZPlacementTable[row][0];
    output->y = gOvl15RowZPlacementTable[row][1];
    output->z = gOvl15RowZPlacementTable[row][2] + column * 600;
    output->direction = (row & 1) ? 1 : 3;
    mode = 5;
    output->mode = mode;
    return mode;
}
