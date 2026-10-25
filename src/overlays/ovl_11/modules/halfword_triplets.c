/* Three-halfword record primitives, also used for timed records whose last
 * word is accumulated. These are not assumed to be geometric vectors.
 * Contracts: ovl11_halfword_triplets.h. Membership: notes/file-groupings.md. */
#include "ovl11_halfword_triplets.h"

s32 Ovl11AddTripletTail(Ovl11TripletTailView *record, s16 delta) {
    s32 sum;

    sum = delta + (u16) record->tail;
    record->tail = sum;
    if ((s16) sum == 0) {
        return 1;
    }
    if ((s16) sum < 0) {
        record->tail = -1;
    }
    return 0;
}

void Ovl11ClearHalfwordTriplet(s16 *values) {
    values[0] = 0;
    values[1] = 0;
    values[2] = 0;
}

void Ovl11SetHalfwordTriplet(s16 *values, s16 first, s16 second, s16 third) {
    values[0] = first;
    values[1] = second;
    values[2] = third;
}

void Ovl11CopyHalfwordTriplet(u16 *destination, u16 *source) {
    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
}
