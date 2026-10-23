#include "common.h"
#include "psyq/memory.h"

u32 func_80012A34(s32 arg0);
s32 ovl_11_func_800F44B4(u16 arg0, s32 *position, s16 *area, s32 index);

s16 ovl_11_func_80110944(s16 area, u16 mode) {
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
    s32 flags;
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
    available = D_800712F8;
    for (i = 0; i < limit; i++) {
        eligible = 0;
        if ((mode & 8) && i < 10) {
            if (ovl_11_func_800F44B4(0x122, position, &foundArea, i) == 1) {
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
    /* This second binding preserves the original untied HIGH/LOW allocation.
     * See notes/research/ovl_11_func_80110944-untied-high-low.md. */
    large = Ovl30_AnimalRecords749F4;
    if (count) {
        return choices[func_80012A34((u16)count)];
    }
    return -1;
}
