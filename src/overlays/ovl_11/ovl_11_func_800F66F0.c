#include "common.h"
#include "game_types.h"
#include "pad_actuator_sequences.h"

void ovl_11_func_800F6680(void);

void func_8001AF70(u16 arg0, u16 arg1);

void ovl_11_func_800F66F0(void) {
    ovl_11_func_800F6680();
    D_80126E40 = 6;
    func_8001AF70(0x71, 1);
    RequestPadActuatorSequence(0, 2);
}
