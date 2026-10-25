#include "common.h"
#include "psyq/libpad.h"

s32 gPadActuatorsEnabled;

void func_80014064(void) {
    PadInitDirect((unsigned char *)&gPadReadBuffers, (unsigned char *)&gPadReadBuffers + 0x22);
    PadSetAct(0, (unsigned char *)&gPadActuatorBuffers, 8);
    PadSetAct(0x10, (unsigned char *)&gPadActuatorBuffers + 8, 8);
    PadStartCom();
    gPadActuatorsEnabled = 1;
}
