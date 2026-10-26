#include "common.h"
#include "game_types.h"
#include "random.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_map_grid_cells.h"
#include "ovl11_cell_actions.h"
#include "ovl11_cell_grid_lookup.h"

s32 ovl_11_func_800E7D4C(s16 mode, s16 arg1, s32 slot) {
    Ovl11D124Entry item;
    s16 enabled[2][6];
    s16 x, y;
    s16 x2, y2;
    s32 i, j;
    s32 area;
    s16 group;
    s32 kind;
    s32 count;
    s32 resultCount;
    s32 available;
    Ovl11D124Entry *entry;

    if (mode == 0) {
        if (arg1 == 0) {
            for (i = 0; i < 25; i++) {
                s32 column;

                for (column = 0; column < 45; column++) {
                    entry = &gMainAreaGrid[i][column];
                    if ((u16)entry->unk0 == 0x40) {
                        entry->unk0 = 0x167;
                        entry->unk4 = 0;
                        entry->unk5 = 0;
                        entry->unk6 = 0;
                    }
                }
            }
        }
    } else if (mode == 1) {
        resultCount = 0;
        for (j = 0; j < 6; j++) {
            enabled[0][j] = 1;
            enabled[1][j] = 1;
        }
        available = ((Ovl11E7D4CState *)D_8006C838)->available;
        for (count = 0; count < 50;) {
            if (available != 0) {
                for (j = 0;; j++) {
                    group = 1;
                    if (j >= 6) {
                        available = 0;
                        break;
                    }
                    if (enabled[0][j] != 0) {
                        group = Rand(2) != 0;
                        break;
                    }
                }
            } else {
                for (j = 5; j >= 0; j--) {
                    enabled[0][j] = 0;
                }
                group = 1;
            }
            for (j = 0; j < 12; j++) {
                if (enabled[j / 6][j % 6] != 0) {
                    break;
                }
            }
            if (j >= 12) {
                return 0;
            }
            for (;;) {
                kind = Rand(6);
                if (enabled[group][kind] == 0) {
                    continue;
                }
                switch (kind) {
                    case 0: item.unk0 = 0x31; break;
                    case 1: item.unk0 = 0x32; break;
                    case 2: item.unk0 = 0x178; break;
                    case 3: item.unk0 = 0x34; break;
                    case 4: item.unk0 = 0x35; break;
                    case 5: item.unk0 = 0x179; break;
                }
                item.unk4 = Ovl11ReadRecordTypeProperty02(item.unk0);
                if (Ovl11FindRandomGridCell(&x, &y, group, &item, 2) == 0) {
                    goto no_match;
                }
                break;
            }
            if (group == 1) {
                entry = &gMainAreaGrid[y][x];
            } else {
                entry = &gAlternateAreaGrid[y][x];
            }
            entry->unk0 = entry->unk2;
            entry->unk4 = 0;
            entry->unk5 = 0;
            entry->unk6 = 0;
            count++;
            goto next_attempt;
        no_match:
            enabled[group][kind] = 0;
        next_attempt:
            ;
        }
    } else if (mode == 2) {
        Ovl11D124Entry *selected;

        for (area = 0; area < 2; area++) {
            for (kind = 0; kind < 6; kind++) {
                group = area != 0;
                switch (kind) {
                    case 0: item.unk0 = 0x31; break;
                    case 1: item.unk0 = 0x32; break;
                    case 2: item.unk0 = 0x178; break;
                    case 3: item.unk0 = 0x34; break;
                    case 4: item.unk0 = 0x35; break;
                    case 5: item.unk0 = 0x179; break;
                }
                item.unk4 = Ovl11ReadRecordTypeProperty02(item.unk0);
                if (Ovl11FindRandomGridCell(&x2, &y2, group, &item, 2) != 0) {
                    if (group == 1) {
                        selected = &gMainAreaGrid[y2][x2];
                    } else {
                        selected = &gAlternateAreaGrid[y2][x2];
                    }
                    gOvl11ValueSlots[slot] = Ovl11ResolveCellItemState((Ovl11CellActionEntry *)selected);
                    selected->unk0 = selected->unk2;
                    selected->unk4 = 0;
                    selected->unk5 = 0;
                    selected->unk6 = 0;
                    return 1;
                }
            }
        }
        return 0;
    }
    if (mode == 1 && resultCount < 50) {
        return 0;
    }
    return 1;
}
