#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_record_placement.h"

void ovl_11_func_800CE034(Ovl11CE034Object *arg0, Ovl11CE034Position *arg1, s32 arg2);
void ovl_11_func_800D666C(s16 arg0);

void ovl_11_func_800CBD78(Ovl11C7010Object *arg0) {
    VECTOR sp10;
    s16 temp_s0;

    temp_s0 = arg0->itemIndex;
    if (D_8006C858[temp_s0].u0.flags & 0x100) {
        ovl_11_func_800CE034((Ovl11CE034Object *) arg0, (Ovl11CE034Position *) &sp10, 2);
        Ovl11RefreshRecordById(temp_s0, (Ovl25SetupStatePair *) ((u8 *) arg0 + 0xFC), &sp10);
    }
    ovl_11_func_800D666C(temp_s0);
    Ovl11ClearHalfwordTriplet(&arg0->itemIndex);
}
