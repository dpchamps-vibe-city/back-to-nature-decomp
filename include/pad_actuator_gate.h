#ifndef PAD_ACTUATOR_GATE_H
#define PAD_ACTUATOR_GATE_H

#include "common.h"

/* Shared software gate for both pads' actuator output, not input polling.
 * Store the raw signed word: zero disables, any nonzero value enables. The
 * getter returns that word unchanged, not a normalized boolean. Pad startup
 * (exe:80014064) sets it to 1; ResetPadInputState does not change it.
 *
 * Setting the gate only changes this word. It does not call the SDK, clear
 * actuator requests/buffers, or stop a motor synchronously. On a subsequent
 * per-pad output update (exe:80014494), zero clears BOTH eight-byte SDK
 * actuator tables. The request scheduler at exe:8001B2CC also rejects new
 * requests while zero. Re-enabling neither resets nor schedules requests.
 * Coordinate with the normal pad service; these accessors add no locking. */
void SetPadActuatorsEnabled(s32 enabled);
s32 GetPadActuatorsEnabled(void);

#endif
