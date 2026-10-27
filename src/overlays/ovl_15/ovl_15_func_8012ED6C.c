#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "psyq/libmcrd.h"
#include "psyq/libmcx.h"
#include "psyq/strings.h"
#include "ovl15_card_service.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);

void ovl_15_func_8012ED6C(void) {
    register s16 *states;
    s32 i;
    s32 status;
    s32 text;

    text = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(text + 0x18), text + 0x14,
                 (s32) &D_80052FC4[gTextBankOffsets[0]]);
    if (gOvl15MenuStateTimer >= 2) {
        for (i = 0; i < 2; i++) {
            states = &gOvl15CardStates[i];
            status = Ovl15ProbeCard((s16)i);
            if (status == 1) {
                status = 0;
            }
            if (status == 7) {
                status = 6;
            }
            if (status != *states) {
                *states = status;
            }
        }
        states = gOvl15CardStates;
        if (gOvl15CardStates[0] == 4 && states[1] == 4) {
            gOvl15MenuState = 2;
        } else {
            gOvl15MenuState = 3;
        }
    }
}
