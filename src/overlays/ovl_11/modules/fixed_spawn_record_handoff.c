/*
 * Fixed spawn-record handoff, ovl_11:8010B05C–8010B0E8.
 * Shared by the action-entry bank and update dispatcher; no private state.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_spawn_record_access.h"
#include "ovl11_fixed_spawn_record_handoff.h"

void Ovl11PublishObjectToFixedSpawnRecord(Ovl11SpawnHandoffObject *object) {
    Ovl11SpawnHandoffObject snapshot;

    snapshot = *object;
    /* Keep the complete object snapshot and all four position ABI words.
     * The fixed-slot writer consumes XYZ but receives an SDK VECTOR by value. */
    Ovl11SetSpawnRecord(0x65, *(VECTOR *)&snapshot.position, snapshot.selector, 0);
}
