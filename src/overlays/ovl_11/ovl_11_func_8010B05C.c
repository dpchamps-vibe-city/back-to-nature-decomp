#include "common.h"
#include "game_types.h"

#include "ovl11_spawn_record_access.h"

void ovl_11_func_8010B05C(Ovl11B05CObject *arg0) {
    Ovl11B05CObject local;

    local = *arg0;
    /* Preserve all four words of the object's position at the SDK boundary. */
    Ovl11SetSpawnRecord(0x65, *(VECTOR *)&local.position, local.selector, 0);
}
