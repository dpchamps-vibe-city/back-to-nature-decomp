#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "pad_actuator_sequences.h"

extern char D_800B8008[];
extern s32 D_800B8608;

/* Menu counter display: PANDO=%d. */
void ovl_08_func_800B8400(void) {
    if (((SomeStruct *)gPad0Input)->field_0x8 & 8) {
        D_800B8608 = D_800B8608 + 1;
        D_800B8608 = (D_800B8608 < 0xB) ? D_800B8608 : 0;
    }
    if (((SomeStruct *)gPad0Input)->field_0x8 & 2) {
        D_800B8608 = D_800B8608 - 1;
        D_800B8608 = (D_800B8608 >= 0) ? D_800B8608 : 0xA;
    }
    if (((SomeStruct *)gPad0Input)->field_0x8 & 0x40) {
        RequestPadActuatorSequence(0, D_800B8608);
    }
    if (((SomeStruct *)gPad0Input)->field_0x8 & 0x80) {
        StopPadActuatorSequence(0);
    }
    FntPrint(D_800B8008, D_800B8608);
}
