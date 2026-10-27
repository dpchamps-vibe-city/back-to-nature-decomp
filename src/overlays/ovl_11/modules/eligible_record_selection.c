/*
 * eligible_record_selection — reconstructed translation unit.
 *
 * Select an eligible record index from area-filtered small/large record
 * pools, excluding active reservations.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "psyq/memory.h"
#include "random.h"
#include "ovl11_spawn_record_access.h"
#include "ovl11_eligible_record_selection.h"

s32 Ovl11ChooseEligibleRecord(s16 area, u16 mode) {
    u8 choices[20];
    s32 position[3];
    s16 foundArea;
    s32 count;
    s32 small;
    s32 limit;
    s32 i;
    s32 eligible;
    Ovl11AnimalSelection *animal;
    u8 *innerBase;
    Ovl11SelectionState *state;
    Ovl30AnimalRecordB8 *large;
    s32 reservationIndex;
    s32 recordOffset;
    u8 *available;

    memset(choices, 0xFF, 20);
    count = 0;
    if (area == 5 || (area != 4 && (mode & 6))) {
        small = 0;
        limit = 20;
    } else {
        small = 1;
        limit = 10;
    }
    available = gRecordSelectionAvailability;
    for (i = 0; i < limit; i++) {
        eligible = 0;
        if ((mode & 8) && i < 10) {
            if (Ovl11ReadSpawnRecord(0x122, position, &foundArea, i) == 1) {
                eligible = foundArea == area;
            }
        }
        if (!small) {
            recordOffset = i * sizeof(Ovl30AnimalRecordB8);
            state = Ovl11_SelectionState;
            /* Keep the root binding separate from the large-array binding. */
            animal = (Ovl11AnimalSelection *)state;
            large = Ovl30_AnimalRecords749F4;
            animal = (Ovl11AnimalSelection *)(recordOffset + (u32)large);
            if (animal->area == area && animal->number != 0 && !(animal->flags & 0x20400)) {
                if ((mode & 1) && area == 5 && *(u8 *)((u32)i + (u32)available) == 0) {
                    eligible = 1;
                }
                if ((mode & 2) && !(animal->flags & 4)) {
                    eligible = 1;
                }
                if ((mode & 4) && (animal->number == 0x162 || animal->number == 0x165)) {
                    if (animal->flags & 0x10000) {
                        if (animal->pregnancy >= 7U) {
                            eligible = 1;
                        }
                    } else if (!(animal->flags & 8)) {
                        if (animal->flags & 0x10) {
                            eligible = 1;
                        }
                    }
                }
            }
        } else {
            animal = (Ovl11AnimalSelection *)&Ovl11_SelectionState->smallAnimals[i];
            if (animal->area == area && animal->number != 0 && animal->number != 0x10A) {
                if ((mode & 1) && area == 4 && Ovl11_SelectionState->smallAvailable[i] == 0) {
                    eligible = 1;
                }
            }
        }
        innerBase = (u8 *)D_8006C838;
        /* A matching active reservation excludes this candidate. */
        for (reservationIndex = 0; reservationIndex < 7; reservationIndex++) {
            if (((Ovl11SelectionState *)(innerBase + reservationIndex * 0xF8))->entries[0].kind == 10 &&
                ((Ovl11SelectionState *)(innerBase + reservationIndex * 0xF8))->entries[0].area == area &&
                ((Ovl11SelectionState *)(innerBase + reservationIndex * 0xF8))->entries[0].index == i &&
                !(((mode & 1) + ((((Ovl11SelectionState *)(innerBase + reservationIndex * 0xF8))->entries[0].flags >> 9) & 1)) & 1)) {
                eligible = 0;
            }
        }
        if (eligible) {
            if (mode == 1) {
                if (Ovl11_SelectionState->index0 == i) {
                    if (Ovl11_SelectionState->selected[0] == 0) {
                        choices[count++] = 20;
                    }
                } else if (Ovl11_SelectionState->index1 == i) {
                    if (Ovl11_SelectionState->selected[1] == 0) {
                        choices[count++] = 21;
                    }
                } else {
                    choices[count++] = i;
                }
            } else {
                choices[count++] = i;
            }
        }
    }
    /* This second binding preserves the original untied HIGH/LOW allocation.
     * See notes/research/ovl_11_func_80110944-untied-high-low.md. */
    large = Ovl30_AnimalRecords749F4;
    if (count) {
        return choices[Rand((u16)count)];
    }
    return -1;
}
