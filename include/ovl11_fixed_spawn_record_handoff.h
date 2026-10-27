#ifndef OVL11_FIXED_SPAWN_RECORD_HANDOFF_H
#define OVL11_FIXED_SPAWN_RECORD_HANDOFF_H

#include "common.h"
#include "game_types.h"

/* Snapshot the complete 0xAC-byte object, then publish selector (+0x30) and
 * unscaled XYZ (+0x38) to fixed spawn slot 1 (kind 0x65). The fourth position
 * word is passed, unused, as required by the SDK VECTOR by-value ABI.
 * Ovl11SetSpawnRecord sets active bit 0 and preserves the slot's other flags
 * and unrelated fields. Requires a valid object and initialized spawn pools;
 * neither object nor selector is validated. Does not change the source object. */
void Ovl11PublishObjectToFixedSpawnRecord(Ovl11SpawnHandoffObject *object);

#endif
