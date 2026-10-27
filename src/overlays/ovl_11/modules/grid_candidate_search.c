/*
 * grid_candidate_search — reconstructed translation unit.
 *
 * Circular grid search from one random start, with three candidate predicates
 * behind a shared item filter. Coordinates are written only on success; both
 * the main and alternate area grids are supported.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "random.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_grid_candidate_search.h"

s32 Ovl11CandidateCellHasMappedItemType(Ovl11D124Entry *arg0);
s32 Ovl11CandidateCellKindIs168Or16A(Ovl11D124Entry *arg0);
s32 Ovl11CandidateCellKindIs168To16B(Ovl11D124Entry *arg0);

s32 Ovl11FindCandidateGridCell(s16 *columnOut, s16 *rowOut, s32 useMainGrid,
                             s32 requireMappedType, s32 requireKind168Or16A) {
    Ovl11GridCellSearchView *view;
    Ovl11D124Entry *base;
    s32 count;
    s32 width;
    s32 (*check)(Ovl11D124Entry *);
    s32 i;
    s32 start;

    if (useMainGrid != 0) {
        base = &gMainAreaGrid[0][0];
        count = 0x465;
        width = 0x2D;
    } else {
        view = (Ovl11GridCellSearchView *)D_8006C838;
        if (view->alternateGridEnabled == 0) {
            return 1;
        }
        base = view->alternateGrid;
        count = 0x31;
        width = 7;
    }

    if (requireMappedType != 0) {
        check = Ovl11CandidateCellHasMappedItemType;
    } else if (requireKind168Or16A != 0) {
        check = Ovl11CandidateCellKindIs168Or16A;
    } else {
        check = Ovl11CandidateCellKindIs168To16B;
    }

    i = Rand(count);
    start = i;
    do {
        if (check(base + i) != 0) {
            *columnOut = i % width;
            *rowOut = i / width;
            return 0;
        }
        if (++i >= count) {
            i = 0;
        }
    } while (i != start);
    return 1;
}

s32 Ovl11CandidateItemPassesBaseFilter(Ovl11D124Entry *cell);

s32 Ovl11CandidateCellHasMappedItemType(Ovl11D124Entry *cell) {
    Ovl11CandidateTypeLookupView *tables;
    s32 item;
    s8 type;

    if (Ovl11CandidateItemPassesBaseFilter(cell) != 0) {
        item = (u16)cell->unk0;
        if (item < 0x31) {
            /* IDs below the excluded band pass this range test. */
        } else if (item < 0x36) {
            return 0;
        } else if (item < 0x17A) {
            if (item >= 0x178) {
                return 0;
            }
        }
        tables = (Ovl11CandidateTypeLookupView *)&D_8006C838;
        type = ((s8 *)tables->itemTable)[(u16)cell->unk0 * 0x28 + 3];
        if (cell->unk4 == *(u16 *)((u8 *)tables->typeTable + type * 0x10 + 2)) {
            return 1;
        }
    }
    return 0;
}

s32 Ovl11CandidateCellKindIs168Or16A(Ovl11D124Entry *cell) {
    s32 eligible;
    s32 matches;

    eligible = Ovl11CandidateItemPassesBaseFilter(cell);
    if (eligible != 0) {
        matches = 0;
        if ((u16)cell->unk2 == 0x168) {
            matches = 1;
        } else if ((u16)cell->unk2 == 0x16A) {
            matches = 1;
        }
        return matches;
    }
    return 0;
}

s32 Ovl11CandidateCellKindIs168To16B(Ovl11D124Entry *cell) {
    s32 eligible;
    s32 matches;

    eligible = Ovl11CandidateItemPassesBaseFilter(cell);
    if (eligible != 0) {
        matches = 0;
        if ((u16)cell->unk2 == 0x168) {
            matches = 1;
        } else if ((u16)cell->unk2 == 0x16A) {
            matches = 1;
        } else if ((u16)cell->unk2 == 0x169) {
            matches = 1;
        } else if ((u16)cell->unk2 == 0x16B) {
            matches = 1;
        }
        return matches;
    }
    return 0;
}

s32 Ovl11CandidateItemPassesBaseFilter(Ovl11D124Entry *cell) {
    s32 flagged;

    flagged = Ovl11ItemHasFlag40(cell->unk0);
    if (flagged == 0) {
        return 0;
    } else {
        return ((u32)0) < ((u32)(cell->unk0 ^ 0x36));
    }
}
