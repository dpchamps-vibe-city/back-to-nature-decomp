/* Whole-grid initialization and day-stage growth maintenance.
 * Keep both grids' loaded item/group views and the normalization joins:
 * seasonal invalidation and ordinary day advancement share those exits.
 * Original entries: ovl_11:800D7B24/7B84/7B8C. The middle, unused zero-return
 * hook has no demonstrated purpose and retains its neutral identity.
 * Membership and evidence-limited boundaries: notes/file-groupings.md. */
#include "ovl11_grid_growth.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "ovl11_grid_entry_transitions.h"
#include "ovl11_map_grid_cells.h"
#include "ovl11_item_table_queries.h"

void Ovl11ResetAlternateAreaGrid(void) {
    s32 rowIndex;
    s32 column;
    char *base;
    Ovl11D124Entry *row;

    rowIndex = 0;
    base = (char *)&gAlternateAreaGrid[0][0];
    for (; rowIndex < 7; rowIndex++) {
        row = (Ovl11D124Entry *)(base + rowIndex * 56);
        for (column = 0; column < 7; column++) {
            row[column].unk0 = 0x167;
            row[column].unk2 = 0x167;
            row[column].unk4 = 0;
            row[column].unk5 = 0;
            row[column].unk6 = 0;
        }
    }
}

s32 ovl_11_func_800D7B84(void) {
    return 0;
}

void Ovl11AdvanceSeasonalGridGrowth(void) {
    u16 rows;
    u16 columns;
    Ovl11D124Entry *entry;
    s32 column;
    s32 hasGrowthFlag;
    s32 layer;
    s32 row;
    s32 value;
    u16 tag;
    u16 nextTag;
    u16 flags;
    u16 *growth;
    Ovl11GrowthState *state;
    Ovl11TileItem *item;
    s32 work;
    s32 itemOffset;
    u16 *limit;

    for (layer = 0; layer < 2; layer++) {
        if (layer == 0) {
            rows = 25;
            columns = 45;
        } else {
            rows = 7;
            columns = 7;
            if (((Ovl11GrowthGlobals *) D_8006C838)->enabled == 0) {
                continue;
            }
        }
        {
            for (row = 0; row < rows; row++) {
                for (column = 0; column < columns; column++) {
                    entry = Ovl11GetMapGridCell(layer, column, (s16) row);
                    if (entry != NULL) {
                        itemOffset = (u16) entry->unk0 * sizeof(*item);
                        item = ((Ovl11GrowthGlobals *) D_8006C838)->items;
                        item = (Ovl11TileItem *) ((u8 *) item + itemOffset);
                        work = item->group;
                        work *= sizeof(*((Ovl11GrowthGlobals *) D_8006C838)->groups);
                        growth = (u16 *) ((u8 *) ((Ovl11GrowthGlobals *) D_8006C838)->groups + work);
                        state = (Ovl11GrowthState *) &entry->unk4;
                        if (layer == 0) {
                            Ovl11TrySeasonalGridTransition(entry);
                        }
                        value = entry->unk0;
                        hasGrowthFlag = Ovl11ItemHasFlag40(value);
                        if (layer == 0) {
                            if (((Ovl11GrowthGlobals *) D_8006C838)->season == 3) {
                                tag = (u16) entry->unk0;
                                if (tag == 0x40) {
                                    Ovl11ResetGridEntry((Ovl11GridResetEntry *) entry);
                                } else if ((tag == 0x36) && (entry->unk4 != 0)) {
                                    entry->unk4 = 1;
                                    state->days = 0;
                                    state->flags = 0;
                                }
                            }
                            if (hasGrowthFlag != 0) {
                                value = growth[0];
                                if (value != ((Ovl11GrowthGlobals *) D_8006C838)->season) {
                                    if (((Ovl11GrowthGlobals *) D_8006C838)->season == 3) {
                                        if ((u16) entry->unk0 != 0x36) {
                                            entry->unk0 = 0x168;
                                            entry->unk2 = 0x168;
                                            state->stage = 0;
                                            state->days = 0;
                                            state->flags = 0;
                                        }
                                        goto advanceGrowth;
                                    }
                                    if (value != 4) {
                                        if ((state->stage != 0) && (value == (((Ovl11GrowthGlobals *) D_8006C838)->season - 1))) {
                                            state->stage = 1;
                                        } else {
                                            entry->unk0 = 0x168;
                                            entry->unk2 = 0x168;
                                            state->stage = 0;
                                            state->flags = 0;
                                        }
                                        state->days = 0;
                                        goto normalizeEntry;
                                    }
                                    goto advanceGrowth;
                                }
                                goto advanceGrowth;
                            }
                            goto normalizeEntry;
                        }
                        if (hasGrowthFlag == 0) {
normalizeEntry:
                            Ovl11NormalizeCellKindPair((u16 *) entry);
                        } else {
advanceGrowth:
                            nextTag = (u16) entry->unk2;
                            if ((nextTag == 0x169) || (nextTag == 0x16B) || ((u16) entry->unk0 == 0x36)) {
                                Ovl11NormalizeCellKindPair((u16 *) entry);
                                if (state->stage != growth[1]) {
                                    flags = (u16) state->flags;
                                    if (flags & 0x8000) {
                                        state->flags = flags & 0x7FFF;
                                    } else {
                                        state->days += 1;
                                    }
                                    limit = growth;
                                    limit += state->stage;
                                    work = limit[3];
                                    if ((u32) work <= (u8) state->days) {
                                        entry->unk2 = 0x168;
                                        state->days = 0;
                                        state->stage += 1;
                                    }
                                    if ((u16) entry->unk0 == 0x36) {
                                        Ovl11ApplyCellNeighborhoodVariants((s16) row, (s16) column, (s16) layer, 0, 1);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
