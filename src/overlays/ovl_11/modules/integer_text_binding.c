/*
 * Formats one literal/value-slot integer and binds the trimmed character
 * sequence to an engine substitution-text slot. The private scratch buffer
 * remains bound after return, so the service is deliberately non-reentrant.
 * See ovl11_integer_text_binding.h for slot and lifetime constraints.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_integer_text_binding.h"

s16 *func_8001A970(s32 value, s16 *output, s32 width);
void func_80017A08(s32 slot, s32 textAddress);

s32 Ovl11BindIntegerText(s16 useValueSlot, s16 textSlot, s32 valueOrSlot) {
    s16 *buffer;
    s16 *text;
    s32 value;
    s32 blanks;

    if (useValueSlot != 0) {
        value = gOvl11ValueSlots[valueOrSlot];
    } else {
        value = valueOrSlot;
    }
    buffer = &gOvl11IntegerBindingText;
    *func_8001A970(value, buffer, 0xA) = -1;
    text = buffer;
    blanks = 0;
    if (gOvl11IntegerBindingText == 0xFFD) {
        do {
            blanks++;
            text++;
        } while (blanks < 0xB && *text == 0xFFD);
    }
    func_80017A08(textSlot & 0xFFFF, (s32)text);
    return 1;
}
