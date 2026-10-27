/*
 * Text/archive slot-marker polling command (overlay 11).
 *
 * Compare a signed requested marker with the executable's unsigned marker.
 * Return 1 for equality. A nonmatching -1 request invalidates the marker by
 * installing 1000 and returns 0; other nonmatches leave it unchanged.
 * Negative requests cannot equal the unsigned getter's result. This command
 * does not itself start a load, consume data or wait for completion.
 *
 * No current C caller is known; retain the original external linkage and
 * signed-halfword argument rather than inventing a private interface.
 */

#include "common.h"

u16 func_80017C30(void);
void func_8001AF34(void);

s32 Ovl11PollTextSlotOrInvalidate(s16 requestedMarker) {
    if (requestedMarker == func_80017C30()) {
        return 1;
    }
    if (requestedMarker == -1) {
        func_8001AF34();
    }
    return 0;
}
