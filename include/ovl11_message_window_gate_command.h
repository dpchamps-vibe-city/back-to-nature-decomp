#ifndef OVL11_MESSAGE_WINDOW_GATE_COMMAND_H
#define OVL11_MESSAGE_WINDOW_GATE_COMMAND_H

#include "common.h"

/* release == 0 sets game flag 3 and returns 0. A nonzero release clears it
 * and returns 1 only once the message window phase is zero; otherwise the
 * flag is unchanged and the result is 0. This does not close the window.
 * Unless modeOverride == -1, also invoke the mode service with 2 on successful
 * release, otherwise 0. modeOverride == 2 forces that request to 2 independently
 * of the gate result. The service's return is discarded. Two trailing object-
 * command operands are ignored. No meaning beyond these mode codes is assumed.
 */
s32 Ovl11MessageWindowGateCommand(s16 release, s16 modeOverride,
                                  s32 reserved2, s32 reserved3);

#endif
