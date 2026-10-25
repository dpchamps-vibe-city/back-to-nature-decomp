#include "common.h"
#include "pad_actuator_gate.h"
#include "sound_driver.h"

void ovl_11_func_800FB608(void);
void ovl_11_func_800FB628(void);

void ovl_11_func_800FF96C(void) {
    s16 *p;
    char *base1;
    char *base2;

    p = (s16 *)D_8006C838;
    if (p[0x7267]) {
        SetPadActuatorsEnabled(1);
    } else {
        SetPadActuatorsEnabled(0);
    }
    base1 = (char *)&D_8006C838;
    base1 += 0x8000;
    if (*(s16 *)(base1 + 0x64D0)) {
        SetSoundStereo();
    } else {
        SetSoundMono();
    }
    base2 = (char *)&D_8006C838;
    base2 += 0x8000;
    if (*(s16 *)(base2 + 0x64D2)) {
        ovl_11_func_800FB608();
    } else {
        ovl_11_func_800FB628();
    }
}
