/*
 * Eight-byte map-entry reset and random transitions.
 *
 * Seasonal and whole-grid passes call these operations on the shared
 * main/alternate grid representation: kind/value halfwords, two state bytes,
 * then a flags halfword. The signed seasonal view and unsigned mutation view
 * deliberately retain their established load/comparison semantics.
 *
 * Every successful random transition requires Rand(bound)==0. A disabled
 * gate consumes no random value; the bound is the low 16 bits, not a percent.
 * No function checks the entry pointer or supplies synchronization.
 * Public contracts are in ovl11_grid_entry_transitions.h; membership and
 * boundary evidence is in notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "random.h"
#include "ovl11_grid_entry_transitions.h"

void Ovl11ResetGridEntry(Ovl11GridResetEntry *entry) {
    entry->unk2 = 0x167;
    entry->unk0 = 0x167;
    entry->unk4 = 0;
    entry->unk5 = 0;
    entry->unk6 = 0;
}

void Ovl11TrySeasonalGridTransition(Ovl11D124Entry *entry) {
    s16 randomBound;

    switch (gCalendarSeason) {
    case 1:
        randomBound = 0x40;
        break;
    case 0:
        randomBound = 0x80;
        break;
    case 2:
        randomBound = 0x80;
        break;
    case 3:
        randomBound = 0;
        break;
    default:
        randomBound = 0;
        break;
    }
    if (randomBound != 0) {
        if ((u32)(entry->unk0 - 0x167) < 3) {
            if (Rand(randomBound) == 0) {
                entry->unk0 = 0x40;
                entry->unk2 = 0x167;
            }
        }
    }
}

void Ovl11TryGridKind3FTransition(u16 *kind, s32 randomBound) {
    if (randomBound != 0) {
        if (*kind == 0x3F) {
            if (Rand(randomBound & 0xFFFF) == 0) {
                *kind = 0x175;
            }
        }
    }
}

s32 ovl_11_func_800D5868(s16 arg0);

void Ovl11TryResetEligibleGridEntry(Ovl11GridResetEntry *entry, s32 randomBound) {
    u16 kind;

    if (randomBound != 0) {
        kind = entry->unk0;
        if (kind != 0x36) {
            if (ovl_11_func_800D5868((s16) kind) == 1) {
                if (Rand(randomBound & 0xFFFF) == 0) {
                    entry->unk0 = 0x168;
                    entry->unk2 = 0x168;
                    entry->unk4 = 0;
                    entry->unk5 = 0;
                    entry->unk6 = 0;
                }
            }
        }
    }
}

s32 ovl_11_func_800D5C90(s16 arg0);

void Ovl11TryRefreshActiveGridEntry(Ovl11GridResetEntry *entry, s32 randomBound) {
    if (randomBound != 0) {
        if (entry->unk0 == 0x36) {
            if (entry->unk4 != 0) {
                if (Rand(randomBound & 0xFFFF) == 0) {
                    entry->unk4 = ovl_11_func_800D5C90(0x36);
                    entry->unk5 = 0;
                    entry->unk6 |= 0x8000;
                }
            }
        }
    }
}
