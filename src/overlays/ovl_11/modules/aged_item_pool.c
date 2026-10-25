/*
 * ovl_11's 99-cell aging item pool (IDs A1, A2 and A3).
 *
 * A queued bit gates aging. IDs encode three age tiers; at age 30 a cell
 * returns to age 20 and may add fresh A3 entries, with fewer attempts as
 * occupancy rises. The scan is deliberately in place, so additions into
 * later slots can be visited before this update finishes.
 *
 * Public contracts: aged_item_pool.h. Membership: notes/file-groupings.md.
 */

#include "common.h"
#include "aged_item_pool.h"

u32 Rand(s32 limit);

/* These support entries retain external linkage and original entry addresses. */
void Ovl11InitItemAge(Ovl11ItemCell *cell, s32 id);
void Ovl11RefreshAgedItemId(Ovl11ItemCell *cell);
void Ovl11TrySpawnAgedItems(Ovl11ItemCell *cell);

s32 Ovl11AddAgedItem(s32 id) {
    Ovl11ItemCell *cell;
    s32 index;

    if ((u32)(id - 0xA1) < 3U) {
        cell = gAgedItemCells;
        for (index = 0; index < 0x63; index++) {
            if (cell->id == 0) {
                break;
            }
            cell += 1;
        }
        if (index != 0x63) {
            cell->id = (u16)id;
            Ovl11InitItemAge(cell, id);
            gAgedItemLastAddCount = Ovl11CountAgedItems();
            return 1;
        }
    }
    return 0;
}

void Ovl11ClearAgedItemPool(void) {
    s32 remaining;
    Ovl11ItemCell *cell;
    char *base;

    cell = gAgedItemCells;
    remaining = 0x62;
    do {
        cell->id = 0;
        remaining -= 1;
        cell += 1;
    } while (remaining >= 0);
    /* Keep the two-stage base: this unit forms the far state offsets through
     * base+0x8000 before applying their low displacement. */
    base = (char *)&D_8006C838;
    base += 0x8000;
    *(s16 *)(base + 0x19E4) = 0;
}

s32 Ovl11UpdateAgedItemPool(void) {
    Ovl11ItemCell *cell;
    u16 *state;
    char *base;
    s32 remaining;

    cell = gAgedItemCells;
    state = (u16 *)D_8006C838;
    if (state[0x48D4] & 1) {
        remaining = 0x62;
        do {
            if (cell->id != 0) {
                cell->age += 1;
                Ovl11RefreshAgedItemId(cell);
                Ovl11TrySpawnAgedItems(cell);
            }
            remaining -= 1;
            cell += 1;
        } while (remaining >= 0);
    }
    base = (char *)&D_8006C838;
    base += 0x8000;
    *(u16 *)(base + 0x11A8) = (u16)(*(u16 *)(base + 0x11A8) & 0xFFFE);
    *(s16 *)(base + 0x19E4) = Ovl11CountAgedItems();
}

void Ovl11QueueAgedItemUpdate(void) {
    u16 *state = (u16 *)D_8006C838;
    state[0x48D4] |= 1;
}

s32 Ovl11TakeAgedItem(void) {
    s32 tier;
    Ovl11ItemCell *cell;
    s32 index;
    s32 id;

    for (tier = 0; tier < 3; tier++) {
        switch (tier) {
        case 0:
            id = 0xA1;
            break;
        case 1:
            id = 0xA2;
            break;
        case 2:
            id = 0xA3;
            break;
        default:
            id = 0xA3;
            break;
        }
        cell = gAgedItemCells;
        for (index = 0; index < 99; index++) {
            if (cell->id == id) {
                cell->id = 0;
                return id;
            }
            cell++;
        }
    }
    return 0;
}

void Ovl11InitItemAge(Ovl11ItemCell *cell, s32 id) {
    switch (id) {
    case 0xA3:
        cell->age = 0;
        break;
    case 0xA2:
        cell->age = 0xA;
        break;
    case 0xA1:
        cell->age = 0x14;
        break;
    }
}

void Ovl11RefreshAgedItemId(Ovl11ItemCell *cell) {
    if (cell->age < 10) {
        cell->id = 0xA3;
    } else if (cell->age < 20) {
        cell->id = 0xA2;
    } else {
        cell->id = 0xA1;
    }
}

void Ovl11TrySpawnAgedItems(Ovl11ItemCell *cell) {
    s32 occupancy;
    s32 attempts;

    /* Count even when this cell is too young; preserve the original call. */
    occupancy = Ovl11CountAgedItems();
    if (cell->age >= 0x1E) {
        attempts = ((s32)Rand(0x64) - occupancy) / 25;
        cell->age = 0x14;
        if (attempts > 0) {
            occupancy = attempts;
            do {
                Ovl11AddAgedItem(0xA3);
                occupancy -= 1;
            } while (occupancy != 0);
        }
    }
}

s32 Ovl11CountAgedItems(void) {
    s32 occupied;
    s32 remaining;
    Ovl11ItemCell *cell;

    occupied = 0;
    cell = gAgedItemCells;
    remaining = 0x62;
    do {
        if (cell->id != 0) {
            occupied += 1;
        }
        remaining -= 1;
        cell += 1;
    } while (remaining >= 0);
    return occupied;
}
