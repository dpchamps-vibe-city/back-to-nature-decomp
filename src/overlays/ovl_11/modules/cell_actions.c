/*
 * Coordinate-based item actions on eight-byte map cells. Callers select
 * these alternatives by action/category; the resolved-record operation is
 * the same kind/value/state/flag protocol without a coordinate lookup.
 *
 * Fields +0/+2 are current/pending kind, +4/+5 state bytes, +6 flags.
 * The signed grid view and unsigned/signed union view remain distinct.
 * Kind IDs and action-category meanings are deliberately not given gameplay
 * names. Original update order, signed-to-unsigned threshold casts and
 * quadrant-specific footprints are retained. See ovl11_cell_actions.h and
 * notes/file-groupings.md for public contracts and grouping evidence.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "random.h"
#include "ovl11_grid_entry_transitions.h"
#include "psyq/libgte.h"
#include "ovl11_map_grid_cells.h"
#include "ovl11_cell_actions.h"
#include "ovl11_cell_geometry.h"
#include "ovl11_item_spawn_placement.h"
#include "ovl11_save_counters.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_cell_grid_lookup.h"

s32 Ovl11TryInstallCellItem(s16 layer, s16 column, s16 row, s16 kind) {
    Ovl11D124Entry *entry;
    s16 texture;

    entry = Ovl11GetMapGridCell((s32) layer, column, row);
    if (entry == NULL) {
        return 0;
    }
    if (((u32) (u16) entry->unk0 - 0x168) >= 2) {
        return 0;
    }
    if (Ovl11ItemHasFlag40(kind) == 0) {
        return 0;
    }
    if ((gCalendarSeason == 3) && (layer == 0)) {
        return 0;
    }
    if ((u16) entry->unk2 == 0x168) {
        entry->unk2 = 0x16A;
        texture = 3;
    } else {
        entry->unk2 = 0x16B;
        texture = 4;
    }
    if (kind == 0x33) {
        if (Rand(0x19) == 0) {
            entry->unk0 = 0x179;
        } else {
            entry->unk0 = 0x178;
        }
    } else {
        entry->unk0 = kind;
    }
    entry->unk4 = 0;
    entry->unk5 = 0;
    entry->unk6 = (u16) entry->unk6 & 0x7FFF;
    gOvl11CellTextureSetters[layer](column, row, texture);
    return 1;
}

s32 Ovl11TryPromoteCellPendingKind(s16 layer, s16 column, s16 row, s16 unused) {
    Ovl11D124Entry *entry;
    s16 texture;

    entry = Ovl11GetMapGridCell((s32) layer, column, row);
    if (entry == NULL) {
        return 0;
    }
    if ((((u16) entry->unk2) & 0xFFFF) != 0x16A) {
        if ((((u16) entry->unk2) & 0xFFFF) != 0x168) {
            return 0;
        }
    }
    if (((u16) entry->unk0 == 0x36) && (entry->unk4 != 0)) {
        return 0;
    }
    if ((gCalendarSeason == 3) && (layer == 0)) {
        return 0;
    }
    if ((u16) entry->unk2 == 0x168) {
        entry->unk2 = 0x169;
        texture = 2;
    } else {
        entry->unk2 = 0x16B;
        texture = 4;
    }
    gOvl11CellTextureSetters[layer](column, row, texture);
    return 1;
}

s32 Ovl11TryResetCellAndSpawnEffect(s16 layer, s16 column, s16 row) {
    Ovl11ItemSpawnPosition position;
    Ovl11D124Entry *entry;
    s16 texture;
    u16 kind;
    u8 state;

    entry = Ovl11GetMapGridCell((s32) layer, column, row);
    if (entry == NULL) {
        return 0;
    }
    if ((((struct Ovl11B264StateView *)D_8006C838)->field_44BA == 3) && (layer == 0)) {
        return 0;
    }
    {
        kind = (u16) entry->unk0;
        state = 0;
        if (Ovl11ItemHasFlag40(entry->unk0) == 1) {
            state = entry->unk4;
        }
        if (Ovl11RecordPropertyHasFlag0800(kind, (u16) state) == 0) {
            return 0;
        }
        if (((u16) entry->unk0 == 0x169) || (texture = 1, ((u16) entry->unk2 == 0x16B))) {
            texture = 2;
            entry->unk0 = 0x169;
            entry->unk2 = 0x169;
        } else {
            entry->unk0 = 0x168;
            entry->unk2 = 0x168;
        }
        if ((layer == 0) && (((struct Ovl11B264StateView *)D_8006C838)->field_E4C8 == 1)) {
            texture = 2;
            entry->unk0 = 0x169;
            entry->unk2 = 0x169;
        }
        entry->unk4 = 0;
        entry->unk5 = 0;
        entry->unk0 = (s16) (u16) entry->unk2;
        entry->unk6 = (u16) entry->unk6 & 0x7FFF;
        Ovl11GetCellCornerPosition(column, row, (Vec3 *)&position, layer);
        position.y = 0;
        Ovl11TrySpawnPositionedItem(position, 0);
        gOvl11CellTextureSetters[layer](column, row, texture);
        Ovl11ApplyCellNeighborhoodVariants(row, column, layer, 1, 0);
        return 1;
    }
}

u16 Ovl11ResolveCellItemState(Ovl11CellActionEntry *entry) {
    s16 signedKind;
    s16 index;
    u16 kind;
    u16 result;
    s16 refreshedState;
    u8 state;

    signedKind = entry->u0.unk0_s;
    kind = entry->u0.unk0;
    if (Ovl11ItemHasFlag8100(signedKind) == 0 && Ovl11ItemHasFlag40(signedKind) == 0) {
        return 0;
    }

    index = (s16)kind;
    if (Ovl11ItemHasFlag40(index) == 1) {
        state = entry->unk4;
        if (state != Ovl11ReadRecordTypeProperty02(index)) {
            return 0;
        }
        if (entry->u0.unk0 == 0x36) {
            return 0;
        }
        result = Ovl11ReadRecordTypeProperty0E(index);
        refreshedState = Ovl11ReadRecordTypeProperty04(index);
        if (refreshedState == -1) {
            entry->unk4 = 0;
            entry->unk5 = 0;
            entry->unk6 = 0;
            entry->u0.unk0 = entry->unk2;
        } else {
            entry->unk4 = refreshedState;
            entry->unk6 |= 0x8000;
        }
        entry->unk5 = 0;
    } else {
        result = kind;
        entry->unk4 = 0;
        entry->unk5 = 0;
        entry->unk6 = 0;
        entry->u0.unk0 = entry->unk2;
    }

    if (gCalendarSeason == 2) {
        if (result == 0x40) {
            result = 0x13A;
        }
    } else if (gCalendarSeason == 3) {
        if (result == 0x3A) {
            result = 0x13B;
        } else if (result == 0x3E) {
            result = 0x13C;
        } else if (result == 0x3F) {
            result = 0x13D;
        }
    }
    return result;
}

s32 Ovl11TryApplyCellActionEffect(s16 layer, s16 column, s16 row) {
    Ovl11ItemSpawnPosition position;
    Ovl11D124Entry *entry;
    u16 kind;
    u8 state;

    entry = Ovl11GetMapGridCell((s32) layer, column, row);
    if (entry != NULL) {
        state = 0;
        if (Ovl11ItemHasFlag40(entry->unk0) == 1) {
            state = entry->unk4;
        }
        if (Ovl11RecordPropertyHasFlag8000((u16) entry->unk0, (u16) state) == 1) {
            if ((u16) entry->unk0 == 0x36) {
                Ovl11AddSaveCounter18(1U, 0U);
                Ovl11SpawnCellEffectSprite((u16) entry->unk0, layer, column, row);
                Ovl11RefreshCellTextureVariant(row, column, layer);
            } else {
                Ovl11SpawnCellEffectSprite((u16) entry->unk0, layer, column, row);
                entry->unk0 = entry->unk2;
                entry->unk4 = 0;
                entry->unk5 = 0;
                entry->unk6 = 0;
            }
            kind = (u16) entry->unk0;
            if ((kind == 0x36) || (kind == 0x40)) {
                Ovl11GetCellCornerPosition(column, row, (Vec3 *)&position, layer);
                position.y = 0;
                Ovl11TrySpawnPositionedItem(position, 1);
            }
            return 1;
        }
    }
    return 0;
}

s32 Ovl11TryClearCellFootprint(s16 layer, s16 column, s16 row, s16 power, s16 amount) {
    s32 changed;
    Ovl11D124Entry *entry;
    u16 kind;
    u8 state;

    changed = 0;
    entry = Ovl11GetMapGridCell((s32) layer, column, row);
    if (entry == NULL) {
        return 0;
    }
    kind = (u16) entry->unk0;
    state = 0;
    if (Ovl11ItemHasFlag40(entry->unk0) == 1) {
        state = entry->unk4;
    }
    if ((Ovl11RecordPropertyHasFlag2000(kind, (u16) state) == 1) && ((u32) amount >= Ovl11ReadRecordPropertyValue0(kind, (u16) state)) && ((u32) power >= Ovl11ReadRecordPropertyValue2(kind, (u16) state))) {
        switch (kind) {
        case 0x3B:
        case 0x3C:
            Ovl11ResetGridEntry((void *) entry);
            Ovl11ResetGridEntry((void *)&gMainAreaGrid[row + 1][column]);
            Ovl11ResetGridEntry((void *)&gMainAreaGrid[row][column + 1]);
            Ovl11ResetGridEntry((void *)&gMainAreaGrid[row + 1][column + 1]);
            Ovl11SpawnCellEffectSprite(kind, layer, column + 1, row + 1);
            break;
        case 0x16C:
        case 0x16F:
            Ovl11ResetGridEntry((void *) entry);
            Ovl11ResetGridEntry((void *)&gMainAreaGrid[row + 1][column]);
            Ovl11ResetGridEntry((void *)&gMainAreaGrid[row][column - 1]);
            Ovl11ResetGridEntry((void *)&gMainAreaGrid[row + 1][column - 1]);
            Ovl11SpawnCellEffectSprite(kind, layer, column, row + 1);
            break;
        case 0x16E:
        case 0x171:
            Ovl11ResetGridEntry((void *) entry);
            Ovl11ResetGridEntry((void *)&gMainAreaGrid[row - 1][column]);
            Ovl11ResetGridEntry((void *)&gMainAreaGrid[row][column + 1]);
            Ovl11ResetGridEntry((void *)&gMainAreaGrid[row - 1][column + 1]);
            Ovl11SpawnCellEffectSprite(kind, layer, column + 1, row);
            break;
        case 0x16D:
        case 0x170:
            Ovl11ResetGridEntry((void *) entry);
            Ovl11ResetGridEntry((void *)&gMainAreaGrid[row - 1][column]);
            Ovl11ResetGridEntry((void *)&gMainAreaGrid[row - 1][column - 1]);
            Ovl11ResetGridEntry((void *)&gMainAreaGrid[row][column - 1]);
            Ovl11SpawnCellEffectSprite(kind, layer, column, row);
            break;
        case 0x3A:
        case 0x3F:
        case 0x175:
            Ovl11SpawnCellEffectSprite(kind, layer, column, row);
        default:
            Ovl11ResetGridEntry((void *) entry);
            gOvl11CellTextureSetters[layer](column, row, 0);
            break;
        }
        changed = 1;
    }
    return changed;
}

s32 Ovl11TryClearCellFootprintScored(s16 layer, s16 column, s16 row, s16 power, s16 amount) {
    s32 changed;
    Ovl11D124Entry *entry;
    u16 kind;
    u8 state;

    changed = 0;
    entry = Ovl11GetMapGridCell((s32) layer, column, row);
    if (entry == NULL) {
        return 0;
    }
    {
        kind = (u16) entry->unk0;
        state = 0;
        if (Ovl11ItemHasFlag40(entry->unk0) == 1) {
            state = entry->unk4;
        }
        if ((Ovl11RecordPropertyHasFlag4000(kind, (u16) state) == 1) && ((u32) amount >= Ovl11ReadRecordPropertyValue0(kind, (u16) state)) && ((u32) power >= Ovl11ReadRecordPropertyValue2(kind, (u16) state))) {
            switch (kind) {
            case 0x3D:
                Ovl11AddSaveCounter19(2U, 0U);
                Ovl11ResetGridEntry((void *) entry);
                Ovl11ResetGridEntry((void *)&gMainAreaGrid[row + 1][column]);
                Ovl11ResetGridEntry((void *)&gMainAreaGrid[row][column + 1]);
                Ovl11ResetGridEntry((void *)&gMainAreaGrid[row + 1][column + 1]);
                Ovl11SpawnCellEffectSprite(0x3DU, layer, column + 1, row + 1);
                break;
            case 0x172:
                Ovl11AddSaveCounter19(2U, 0U);
                Ovl11ResetGridEntry((void *) entry);
                Ovl11ResetGridEntry((void *)&gMainAreaGrid[row + 1][column]);
                Ovl11ResetGridEntry((void *)&gMainAreaGrid[row][column - 1]);
                Ovl11ResetGridEntry((void *)&gMainAreaGrid[row + 1][column - 1]);
                Ovl11SpawnCellEffectSprite(0x3DU, layer, column, row + 1);
                break;
            case 0x174:
                Ovl11AddSaveCounter19(2U, 0U);
                Ovl11ResetGridEntry((void *) entry);
                Ovl11ResetGridEntry((void *)&gMainAreaGrid[row - 1][column]);
                Ovl11ResetGridEntry((void *)&gMainAreaGrid[row][column + 1]);
                Ovl11ResetGridEntry((void *)&gMainAreaGrid[row - 1][column + 1]);
                Ovl11SpawnCellEffectSprite(0x3DU, layer, column + 1, row);
                break;
            case 0x173:
                Ovl11AddSaveCounter19(2U, 0U);
                Ovl11ResetGridEntry((void *) entry);
                Ovl11ResetGridEntry((void *)&gMainAreaGrid[row - 1][column]);
                Ovl11ResetGridEntry((void *)&gMainAreaGrid[row - 1][column - 1]);
                Ovl11ResetGridEntry((void *)&gMainAreaGrid[row][column - 1]);
                Ovl11SpawnCellEffectSprite(0x3DU, layer, column, row);
                break;
            case 0x3E:
                Ovl11AddSaveCounter19(1U, 0U);
                Ovl11ResetGridEntry((void *) entry);
                Ovl11SpawnCellEffectSprite(0x3EU, layer, column, row);
                break;
            default:
                Ovl11ResetGridEntry((void *) entry);
                break;
            }
            changed = 1;
        }
    }
    return changed;
}
