/* Date-dependent message lookup over a private thirteen-threshold table.
 * The input is the engine's season/day pair; the caller uses the result as
 * an entry in message archive 2. This service has no state changes.
 * Original ovl_11:80121204–801212C4; contract: ovl11_date_message.h. */
#include "common.h"
#include "ovl11_date_message.h"

s32 Ovl11GetDateMessageEntry(void) {
    struct Ovl11ConditionEvaluationWork *state;
    s16 *threshold;
    s16 previous;
    s16 current;
    s16 shiftedDay;
    s32 entry;
    u32 index;

    state = (struct Ovl11ConditionEvaluationWork *)D_8006C838;
    shiftedDay = (state->date[1] * 30 + state->date[2] + 20) % 120;
    index = 0;
    previous = -1;
    threshold = &gOvl11DateMessageThresholds[0];
    entry = 0x1BD0000;
    current = *threshold;
    while (1) {
        if (previous < shiftedDay && current >= shiftedDay) {
            return entry >> 16;
        }
        threshold++;
        index++;
        /* Keep the fixed-point message accumulator's signed-word shape. */
        entry += 0x10000;
        if (index >= 13) {
            return 0x1BD;
        }
        previous = current;
        current = *threshold;
    }
}
