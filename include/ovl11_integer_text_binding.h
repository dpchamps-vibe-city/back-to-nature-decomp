#ifndef OVL11_INTEGER_TEXT_BINDING_H
#define OVL11_INTEGER_TEXT_BINDING_H

#include "common.h"

/* useValueSlot != 0 resolves valueOrSlot through the unchecked shared value
 * slots; otherwise the full s32 is formatted directly. Formats ten engine
 * character codes, appends FFFF and skips leading 0FFD blanks. The formatter
 * reserves the first position for a minus sign on negative input.
 *
 * textSlot is interpreted as u16 and clamped to 10 by the engine binding
 * service (valid slots 0..10). This stores a POINTER, not a copy: the next
 * call overwrites text bound by any previous call. Consume it before then.
 * Always returns 1; neither value-slot nor numeric-range errors are reported.
 * Uses the existing formatter's arithmetic, including its signed limits. */
s32 Ovl11BindIntegerText(s16 useValueSlot, s16 textSlot, s32 valueOrSlot);

#endif
