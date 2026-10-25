/*
 * Latched, state-dependent selection messages for ovl_11.
 *
 * The public reset/request/service interface is in ovl11_selection_message.h.
 * The selector uses persistent byte variants and two repeat counters in main
 * RAM; the request phase/category live in the overlay. Category meanings and
 * the threshold's units remain unknown. No helper gains private linkage merely
 * because its callers now share this file. Membership/boundaries are recorded
 * in notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_selection_message.h"

s32 func_8002261C(s32 arg0, s32 arg1);
s32 GetVal8005E5B4(void);
s32 func_8001AF44(u32 arg0);
s32 Ovl11AdvanceSelectionMessageVariant(s32 category);
s32 Ovl11CountSelectionMessageItems(void);

void Ovl11ResetSelectionMessage(void) {
    gOvl11SelectionMessagePhase = 0;
    gOvl11SelectionMessageCategory = 0;
}

void Ovl11RequestSelectionMessage(s32 category) {
    s32 selectedCategory;

    selectedCategory = category;
    if (gOvl11SelectionMessagePhase == 0) {
        gOvl11SelectionMessagePhase = 1;
        if (selectedCategory < 0) {
            selectedCategory = 0;
        }
        if (selectedCategory >= 10) {
            selectedCategory = 9;
        }
        gOvl11SelectionMessageCategory = selectedCategory;
    }
}

void Ovl11UpdateSelectionMessage(void) {
    if (((struct Ovl11SpriteWork *) &D_8007AFF0)->header[1] == 0x15) {
        switch ((u32) gOvl11SelectionMessagePhase) {
        case 0:
            return;
        case 1:
            func_8002261C(2, Ovl11AdvanceSelectionMessageVariant(gOvl11SelectionMessageCategory));
            gOvl11SelectionMessagePhase = 2;
            break;
        case 2:
            if (GetVal8005E5B4() == 2) {
                gOvl11SelectionMessagePhase = 3;
            }
            break;
        case 3:
            gOvl11SelectionMessagePhase = 0;
            break;
        }
    }
}

/* Internal caller supplies 0..9. The absolute byte view and work-area view
 * alias the same storage. Keep the pre-update variant separate: some branches
 * return it, some return the newly stored byte, and category 3 may reverse it.
 * Replacing the return with a fresh load would change message selection. */
s32 Ovl11AdvanceSelectionMessageVariant(s32 category) {
    u8 *state;
    s32 variant;
    s16 messageBase;
    s16 mode;
    s32 firstCount;
    s32 secondCount;
    s32 result;
    u8 selected;
    u8 *firstCounter;
    u8 *secondCounter;

    state = &gSelectionMessageStates.states[category];
    variant = ((struct Ovl11StateSelection *) D_8006C838)->states[category];
    messageBase = gOvl11SelectionMessageBases[category];
    mode = ((struct Ovl11StateSelection *) D_8006C838)->mode;
    if (variant != 0) {
        switch (category) {
        case 0:
            if (func_8001AF44(0x64) == 1) {
                *state = (u8) (variant == 2 ? 1 : 2);
                variant = *state;
            } else {
                *state = 1;
            }
            break;
        case 1:
            if (func_8001AF44(0x63) == 1) {
                *state = (u8) (variant == 2 ? 1 : 2);
                variant = *state;
            } else {
                *state = 1;
            }
            break;
        case 2:
            if (func_8001AF44(0x65) == 1) {
                *state = (u8) (variant == 2 ? 1 : 2);
                variant = *state;
            } else {
                *state = 1;
                variant = 1;
            }
            break;
        case 3:
            if (func_8001AF44(0x66) == 1) {
                *state = (u8) (variant == 5 ? 3 : variant + 1);
                variant = 6 - *state;
            } else if (Ovl11CountSelectionMessageItems() >= 10) {
                *state = (u8) (variant == 2 ? 1 : 2);
                variant = *state;
            } else {
                *state = 1;
            }
            break;
        case 4: {
            s32 increment;

            increment = 2;
            if (variant == increment) {
                *state = 1;
            } else {
                increment = variant + 1;
                *state = increment;
            }
            break;
        }
        case 5: {
            s32 increment;

            increment = 5;
            if (variant == increment) {
                *state = 1;
            } else {
                increment = variant + 1;
                *state = increment;
            }
            break;
        }
        case 6: {
            s32 selected;
            u8 next;

            if (mode == 0) {
                variant = 5;
                break;
            }
            selected = 3;
            if (mode == 2) {
                variant = 2;
                break;
            }
            switch (variant) {
            case 1:
                *state = selected;
                break;
            case 3:
                next = 4;
                *state = next;
                break;
            case 4:
            default:
                *state = 1;
                break;
            }
            break;
        }
        case 7: {
            s32 increment;

            increment = 7;
            if (variant == increment) {
                *state = 1;
            } else {
                increment = variant + 1;
                *state = increment;
            }
            break;
        }
        case 8: {
            s32 increment;

            increment = 3;
            if (variant == increment) {
                *state = 1;
            } else {
                increment = variant + 1;
                *state = increment;
            }
            break;
        }
        case 9:
            firstCount = ((struct Ovl11StateSelection *) D_8006C838)->counters[0];
            secondCount = ((struct Ovl11StateSelection *) D_8006C838)->counters[1];
            firstCounter = &gSelectionMessageRepeatCounters.first;
            result = func_8001AF44(0x13D);
            secondCounter = &gSelectionMessageStates.second;
            if (result == 1 || func_8001AF44(0x143) == 1) {
                if (firstCount >= 2) {
                    *state = (u8) (variant == 4 ? 3 : 4);
                } else {
                    *firstCounter = (u8) (*firstCounter + 1);
                    selected = 3;
                    goto store_selected;
                }
                variant = *state;
            } else if (((struct Ovl11StateSelection *) D_8006C838)->threshold > 0xD6D7) {
                if (secondCount >= 2) {
                    *state = (u8) (variant == 6 ? 5 : 6);
                } else {
                    *secondCounter = (u8) (*secondCounter + 1);
                    selected = 5;
store_selected:
                    *state = selected;
                }
                variant = *state;
            } else {
                *state = (u8) (variant == 2 ? 1 : 2);
            }
            break;
        }
    } else {
        result = 1;
        *state = result;
    }
    return variant + messageBase;
}

/* Category 3's occupancy gate. Counts IDs only, not ages, in the 99-cell
 * aged-item pool. This local copy remains distinct from Ovl11CountAgedItems. */
s32 Ovl11CountSelectionMessageItems(void) {
    s32 count;
    s32 remaining;
    u16 *id;

    count = 0;
    id = (u16 *)gAgedItemCells;
    remaining = 0x62;
    do {
        if (*id != 0) {
            count += 1;
        }
        remaining -= 1;
        id += 2;
    } while (remaining >= 0);
    return count;
}
