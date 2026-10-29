/*
 * Formats one literal/value-slot integer and binds the trimmed character
 * sequence to an engine substitution-text slot. The private scratch buffer
 * remains bound after return, so the service is deliberately non-reentrant.
 * See ovl11_integer_text_binding.h for slot and lifetime constraints.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_integer_text_binding.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libapi.h"
#include "psyq/libgpu.h"
#include "text_engine.h"

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
    *WriteDecimalText(value, buffer, 0xA) = -1;
    text = buffer;
    blanks = 0;
    if (gOvl11IntegerBindingText == 0xFFD) {
        do {
            blanks++;
            text++;
        } while (blanks < 0xB && *text == 0xFFD);
    }
    SetTextSourcePointer(textSlot & 0xFFFF, (s32)text);
    return 1;
}
