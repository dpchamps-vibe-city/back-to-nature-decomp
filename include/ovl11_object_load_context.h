#ifndef OVL11_OBJECT_LOAD_CONTEXT_H
#define OVL11_OBJECT_LOAD_CONTEXT_H

#include "common.h"

/* ovl_11 must be resident; the current-object pointer, far-work sprite header
 * and record pointers must already be initialized. These synchronous helpers
 * neither read the archive nor allocate resources. They mutate shared work
 * and are not reentrant. Entry and exit are NOT inverse save/restore calls.
 *
 * Entry sets game flag 3, conditionally enters position control and resets
 * flags 4/5/B0, resets the work timed-sprite effect, writes sprite-source
 * state 1 and clears work +4450 bit 2000. The loader uses it before a load
 * and again on release when more record slots remain queued.
 * Original identity: ovl_11:800E5294. */
void Ovl11EnterObjectLoadContext(void);

/* Clear game flag 3, select position record -2 and release position control.
 * Clear pending-selection/work flags, conditionally request the no-empty-slot
 * state, and refresh either record whose ID is 0x36. This does not unload
 * sprite data, clear the load queue or restore an earlier work snapshot.
 * Original identity: ovl_11:800E537C. */
void Ovl11LeaveObjectLoadContext(void);

#endif
