#include "common.h"
#include "game_types.h"
#include "pad_actuator_sequences.h"
#include "game_flags.h"

void ovl_11_func_800F6680(void);

void ovl_11_func_800F6734(void) {
    ovl_11_func_800F6680();
    D_80126E40 = 7;
    SetGameFlag(0x70, 1);
    RequestPadActuatorSequence(0, 2);
}
