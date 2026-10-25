#ifndef PAD_INPUT_UPDATE_H
#define PAD_INPUT_UPDATE_H

#include "common.h"

/* Poll both SDK controller ports and update their 0x38-byte PadInputState
 * records at gPadInputStateStorage. Boot must have started pad communication
 * and bound gPad0Input/gPad1Input to those records. No allocation, VSync wait,
 * or elapsed-time measurement is performed.
 *
 * Each processed port sends its stored actuator requests to the output
 * formatter, reads active-high held/new-press masks and stick magnitude,
 * and advances two independent repeat streams. Timings count calls, scaled
 * by update mode 2/3; see pad_input_setup.h for delay/interval constraints.
 * Any held-mask change restarts both streams and emits only new presses.
 *
 * Disconnect/find-pad normally mark actuator alignment pending and clear
 * transient input; SDK request-info/execute-command states also clear it.
 * Stable ports cache the extended mode ID while alignment is pending;
 * successful alignment clears pending and retries the same port. These
 * input clears preserve timing pairs and actuator requests.
 * The existing external-input flag bypasses disconnect clearing and can
 * substitute pad-0 buttons (with 0x20/0x40 swapped) unless Start is held.
 * Caller must provide valid external-input context when that flag is set.
 * This routine does not start/stop communication or reset repeat timings. */
void PollPadInputs(void);

#endif
