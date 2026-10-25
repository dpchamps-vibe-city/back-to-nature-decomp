/* Independent actuator-sequence test page. PADNO is the original display
 * label, but the counter actually selects an 11-entry sequence preset,
 * always requested on software slot 0. See ovl08_actuator_test_menu.h. */
#include "common.h"
#include "ovl08_actuator_test_menu.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "pad_actuator_sequences.h"

void Ovl08UpdateActuatorTestMenu(void) {
    if (gPad0Input->pressedButtons & 8) {
        gOvl08ActuatorTestSequenceIndex = gOvl08ActuatorTestSequenceIndex + 1;
        gOvl08ActuatorTestSequenceIndex = (gOvl08ActuatorTestSequenceIndex < 0xB) ? gOvl08ActuatorTestSequenceIndex : 0;
    }
    if (gPad0Input->pressedButtons & 2) {
        gOvl08ActuatorTestSequenceIndex = gOvl08ActuatorTestSequenceIndex - 1;
        gOvl08ActuatorTestSequenceIndex = (gOvl08ActuatorTestSequenceIndex >= 0) ? gOvl08ActuatorTestSequenceIndex : 0xA;
    }
    if (gPad0Input->pressedButtons & 0x40) {
        RequestPadActuatorSequence(0, gOvl08ActuatorTestSequenceIndex);
    }
    if (gPad0Input->pressedButtons & 0x80) {
        StopPadActuatorSequence(0);
    }
    FntPrint(gOvl08ActuatorTestFormat, gOvl08ActuatorTestSequenceIndex);
}
