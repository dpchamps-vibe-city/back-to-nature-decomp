#include "common.h"

#include "game_types.h"
#include "ovl11_halfword_triplets.h"

void ovl_11_func_800D666C(s16 arg0);

void ovl_11_func_800CCF14(Ovl11TripletTailView *arg0) {
    if (Ovl11AddTripletTail(arg0, -1) != 0) {
        ovl_11_func_800D666C(*(s16 *) ((u8 *) arg0 + 0));
        Ovl11ClearHalfwordTriplet((s16 *) arg0);
    }
}
