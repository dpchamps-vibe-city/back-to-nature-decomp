#include "common.h"
#include "game_types.h"
#include "pad_actuator_sequences.h"
#include "game_flags.h"

void ovl_11_func_800F6680(void);

void ovl_11_func_800F66F0(void) {
    ovl_11_func_800F6680();
    D_80126E40 = 6;
    SetGameFlag(0x71, 1);
    RequestPadActuatorSequence(0, 2);
}
