/*
 * special_counter_slot_command — choose a maximum from five event slots.
 * Candidate storage and boundary evidence are recorded in notes/file-groupings.md.
 */

#include "common.h"
#include "random.h"
#include "game_flags.h"
#include "ovl11_record_action_predicate.h"
#include "ovl11_special_counter_slot_command.h"
#include "ovl11_progress_gate.h"

s32 Ovl11ChooseSpecialCounterSlotCommand(s16 skipFlagFilter, s16 resultSlot) {
    struct Ovl11CounterSlotCandidates choices = gOvl11SpecialCounterSlotCandidates;
    s16 count = 0;
    s16 i;
    u16 highest;
    u16 value;
    char *base;
    char *base2;
    char *p;
    char *p2;

    if (skipFlagFilter == 0) {
        for (i = 0; i < 4; i++) {
            if (GetGameFlag((u16)(i + 0x21)) == 1) {
                choices.slots[i] = -1;
                count++;
            }
        }
        if (Ovl11TestProgressGate(0) == 1) {
            choices.slots[4] = -1;
            count++;
        }
    }
    if (choices.slots[4] != -1) {
        if (Ovl11IsRecordActionAllowed(choices.slots[4], 0, 0, 0) == 0) {
            choices.slots[4] = -1;
            count++;
        }
    }
    highest = 0;
    if (count == 5) {
        return 0;
    }
    count = 0;
    for (i = 0; i < 5; i++) {
        if (choices.slots[i] != -1) {
            base = (char *)&D_8006C838;
            p = choices.slots[i] * 0x1D4 + base;
            value = *(u16 *)(p + 0x8000 + 0x19EC);
            if (highest < value) {
                highest = value;
                count = 1;
            } else if (highest == value) {
                count++;
            }
        }
    }
    count = Rand((u16)count);
    for (i = 0; i < 5; i++) {
        if (choices.slots[i] != -1) {
            base2 = (char *)&D_8006C838;
            p2 = choices.slots[i] * 0x1D4 + base2;
            if (highest == *(u16 *)(p2 + 0x8000 + 0x19EC)) {
                if (count == 0) {
                    break;
                }
                count--;
            }
        }
    }
    if (resultSlot != -1) {
        gOvl11ValueSlots[resultSlot] = choices.slots[i];
    }
    return 1;
}
