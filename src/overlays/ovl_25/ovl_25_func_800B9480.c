#include "common.h"
#include "random.h"
#include "ovl25_record_state_timer.h"

s32 ovl_25_func_800B977C(s16 *arg0, s16 arg1);
void ovl_25_func_800B95B0(s16 *arg0);

void ovl_25_func_800B9480(s16 *arg0) {
    s16 random;
    s16 *state;
    s32 value;

    if (ovl_25_func_800B977C(arg0, 4) == 1) {
        state = (s16 *)&D_800BFE44_view;
        random = Rand((u16)(120 / state[0x26C])) + 1;
        value = state[0x267] * 30;
        if (random >= value / (state[0x26C] * 100)) {
            Ovl25SetRecordState(arg0, 3, 60);
            value = random - 2;
            arg0[3] = value * 30 / state[0x26A];
            return;
        } else {
            Ovl25SetRecordState(arg0, 2, random * (u16)state[0x26A]);
        }
    } else {
        ovl_25_func_800B95B0(arg0);
    }
}
