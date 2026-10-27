#ifndef OVL11_CLOCK_EVENT_TRANSITION_COMMAND_H
#define OVL11_CLOCK_EVENT_TRANSITION_COMMAND_H

#include "common.h"

/* ovl_11:800E7798. mode 1 writes scene mode C, sets work+5234 bit 400
 * and game flag 47. Modes 2/3 request relative hours / target hour:00 via
 * the clock command; its failure result is ignored. Other modes skip this.
 * All modes: controlFlags bit 1 clears display-enable bit 100 in event
 * slots 1..36 (broadcast selector 2A, mode 2),
 * clear game flags 4/5/B0, optionally set work+4450 bit 8, and write 2 to
 * work+7A74. controlFlags bit 0 is forwarded to an ignored operand; other
 * bits have no effect. Return 1 immediately, not transition completion.
 * Requires initialized shared work, flags and clock state. */
s32 Ovl11ClockEventTransitionCommand(s16 mode, s16 hours, s32 setWorkFlag,
                                    s32 controlFlags);

#endif
