#include "common.h"

INCLUDE_ASM("build/ovl_11/asm/nonmatchings/ovl_11_func_80110944", ovl_11_func_80110944);


/* PARKED by /auto_decompilation_loop on 2026-10-19T10:05:45.990Z.
 * Reason: escalation-exhausted.
 * Escalation reached: gpt-6.1-sol.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_11_func_80110944.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"
#include "psyq/memory.h"

u32 func_80012A34(s32 arg0);
s32 ovl_11_func_800F44B4(u16 arg0, s32 *position, s16 *area, s32 index);

s32 ovl_11_func_80110944(s16 area, u16 mode) {
    u8 choices[20];
    s32 position[3];
    s16 foundArea;
    s32 count;
    s32 small;
    s32 limit;
    s32 i;
    s32 eligible;
    Ovl11AnimalSelection *animal;
    Ovl11AnimalSelection *smallAnimal;
    u8 *innerBase;
    Ovl11SelectionState *state;
    Ovl30AnimalRecordB8 *large;
    s32 flags;
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
    available = D_800712F8;
    for (i = 0; i < limit; i++) {
        eligible = 0;
        if ((mode & 8) && i < 10) {
            if (ovl_11_func_800F44B4(0x122, position, &foundArea, i) == 1) {
                eligible = foundArea == area;
            }
        }
        if (!small) {
            state = Ovl11_SelectionState;
            large = state->largeAnimals;
            animal = (Ovl11AnimalSelection *)&large[i];
            if (animal->area != area) {
                goto scan_entries;
            }
            if (animal->number == 0) {
                goto scan_entries;
            }
            if (animal->flags & 0x20400) {
                goto scan_entries;
            }
            {
                if ((mode & 1) && area == 5 && available[i] == 0) {
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
            smallAnimal = (Ovl11AnimalSelection *)&Ovl11_SelectionState->smallAnimals[i];
            if (smallAnimal->area == area && smallAnimal->number != 0 && smallAnimal->number != 0x10A) {
                if ((mode & 1) && area == 4 && Ovl11_SelectionState->smallAvailable[i] == 0) {
                    eligible = 1;
                }
            }
        }
scan_entries:
        innerBase = (u8 *)D_8006C838;
        for (flags = 0; flags < 7; flags++) {
            if (((Ovl11SelectionState *)(innerBase + flags * 0xF8))->entries[0].kind == 10 &&
                ((Ovl11SelectionState *)(innerBase + flags * 0xF8))->entries[0].area == area &&
                ((Ovl11SelectionState *)(innerBase + flags * 0xF8))->entries[0].index == i &&
                !(((mode & 1) + ((((Ovl11SelectionState *)(innerBase + flags * 0xF8))->entries[0].flags >> 9) & 1)) & 1)) {
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
    if (count) {
        return choices[func_80012A34((u16)count)];
    }
    return -1;
}
#endif
