#ifndef OVL11_OBJECT_RECORD_STATE_H
#define OVL11_OBJECT_RECORD_STATE_H

#include "common.h"
#include "game_types.h"

/* Seven 0xF8-byte records in gObjectRecordPool. All services use the existing
 * saved work/asset tables; no allocation or new ownership. Different partial
 * views deliberately retain each site's signed/unsigned halfword reads. */

/* Clear all seven records, install their bank-relative initial bytes,
 * per-record indices, positions and area/room words; seed five work indices.
 * Calls the reset below seven times, consuming seven Rand(300) results. */
void Ovl11InitObjectRecordPool(void);

/* Frame service: work flag +10 bit 100 suppresses everything; record +B8
 * bit 20 skips that record. Message completion, mode/direction, sprite and
 * timed-effect work retain their distinct area and +C flag gates. Units are
 * service calls, NOT seconds. Requires initialized source/header pointers. */
void Ovl11UpdateObjectRecordPool(void);

/* Visit the same seven enabled records. Area actions require advancing
 * clock hour 6..20, clear record +B8 bit 40, and daily condition != 3/4.
 * Counter updates clamp +16 to 0..255, update flag-selected counters, consume
 * Rand(300), and select mode/placement using the remaining +B4 countdown. */
void Ovl11DispatchObjectPoolAreaActions(void);
void Ovl11AdvanceObjectPoolCounters(void);

/* mode indexes both 14-entry banks without a range check. A guard can mutate
 * object/work/message state even when it rejects entry. Return its result
 * (0 if absent); -1 prevents committing the mode. Otherwise a non-NULL frame
 * handler permits mode assignment, phase/timer reset and +B8 bit 800 clear.
 * For a guarded mode, re-requesting the current mode with object flag +34
 * bit 800 skips the guard and returns -1. The partial entry view is not a
 * new record allocation. */
s32 Ovl11SetObjectRecordMode(Recon_ovl_11_func_8010CE80_A0View *object,
                            s32 mode);

/* Legacy address must cover 0xF8 writable bytes. Zero the record, copy the
 * work area's unaligned eight-byte pair to +1A, install sentinels/defaults,
 * reset its +A8 timed effect and set +2C = Rand(300). Always returns 0. */
s32 Ovl11ResetObjectRecord(s32 objectAddress);

#endif
