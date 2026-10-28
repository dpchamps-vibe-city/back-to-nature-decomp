#ifndef OVL25_SCENE_STATE_MACHINE_H
#define OVL25_SCENE_STATE_MACHINE_H

#include "common.h"

/* Engine-installed frame callback for five phases: staged loading, fade-in,
 * mode update, exit request and teardown. The mode table selects event,
 * dialogue or active-record processing. All selectors are unchecked; scene
 * entry must initialize the phase, load step and mode before dispatch.
 *
 * Saves/restores text draw mode (3 inside), then clears work flag 08000000.
 * Loading advances one stage per call and waits for sound transfer. Active
 * calls update input, target, records and completion in order; paused calls
 * draw without those updates. Drawing the sequence sprites also advances them.
 * Exit waits for screen/volume fades, stops audio, requests resident scene 6
 * and clears background RGB. Services/buffers must already be initialized.
 * Shared state is non-reentrant; timers count calls, not milliseconds.
 */
void Ovl25DispatchScenePhase(void);

/* Seed six scene records and two auxiliary records, each 0x78 bytes apart,
 * using the SECOND banks of the initial-record tables. Scene record +0 is
 * its index, +2 is 1, +4/+6/+8 are zero, +0C is 0x32000, and eight bytes at
 * +10 come from the table. Auxiliary records get their index/table values
 * and markers A/B. Other fields remain unchanged; no storage is allocated.
 * Does not reset the surrounding mode, pause, result, timer or sound state.
 * Requires initialized tables and record storage; there are no failure results.
 */
void Ovl25SeedInitialSceneRecords(void);

/* Reseed those same records from the FIRST banks. Unlike the operation above,
 * writes scene +2 as zero and retains +6/+8. The index, +4, +0C and table copy
 * are written again. Same prerequisites and shared-state constraints apply.
 */
void Ovl25ResetSceneRecords(void);

#endif
