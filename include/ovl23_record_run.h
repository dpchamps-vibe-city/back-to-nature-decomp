#ifndef OVL23_RECORD_RUN_H
#define OVL23_RECORD_RUN_H

#include "common.h"

/* Resident overlay-23 scene entry (original 800B7EA4), called once per scene
 * update. gSceneSubphase must be 0..4; during subphase 2 the work aggregate's
 * signed leading halfword must be 0..6. Both table indices are unchecked.
 * Temporarily selects text draw mode 3, dispatches, then restores the old mode.
 *
 * Setup loads sprite/sound banks, starts a sequence and initializes six actors,
 * six decision records and their trails. The message stages use archive 4,
 * entries 0x2A..0x2E, and can remain pending across updates. The interactive
 * stage consumes pad state, advances Q12 movement and update-call timers,
 * draws packets, and can pause sprite updates or change work-record values.
 * Exit waits for screen/master-volume fades before stopping sound, rewinding
 * load slots and requesting scene 6. Requires initialized render/packet work,
 * controller, message and sound services; uses shared state, not reentrant.
 *
 * The trail sub-interface remains in ovl23_record_trails.h. Other scene helper
 * linkage is retained, but those functions are not independent public APIs. */
void Ovl23DispatchRecordRunSubphase(void);

#endif
