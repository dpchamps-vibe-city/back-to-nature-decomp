#ifndef OVL11_OBJECT_EVENTS_H
#define OVL11_OBJECT_EVENTS_H

#include "common.h"
#include "ovl11_object_event_pool.h"
#include "ovl11_object_event_script.h"

/* Object-event service, ovl_11:800C19B0..800C401C. All entry points share the
 * 37 x 0x1D4-byte record pool, schedule/command tables and sprite state.
 * Initialize those tables and the shared scene state first. The pool/script
 * headers describe allocation, replay ticks, indices and fixed-point motion.
 * No operation is reentrant; rendering also requires active packet work. */

/* Reconstruct eligible records for the current scene/time using script replay.
 * Temporarily clears the live-mode flag, resets the shared script tick for
 * each record, selects sprite sources and replays elapsed schedule time.
 * Always leaves live mode enabled. It does not reset/populate the pool. */
void Ovl11RebuildObjectEventsForTime(void);

/* Scan the FIRST 35 slots for eligible reactions near the supplied actor.
 * Actor view: u16 heading-index +0x38, s16 scene +0xFE, s32 X/Z +0x100/+0x108.
 * Uses squared (delta+1) distance and a strict angular window of +/-600 SDK
 * angle units. May start multiple reactions; returns ZERO unconditionally,
 * not a selected record or success Boolean. Advances Rand for eligible slots. */
s32 Ovl11TriggerNearbyObjectReactions(u8 *actor);

/* Low signed 16 bits of slotIndex classify {3,7,9,14,20}. Used by allocation,
 * counter maintenance and action predicates; this tests SLOT, not stored ID. */
s32 Ovl11IsSpecialCounterSlot(s32 slotIndex);

/* Under the far-object ID==0x13 gate, decrement both pool counters toward zero
 * and advance a separate shared counter up to 15. Retains signed halfword
 * interpretation of the first counter. */
void Ovl11CountDownObjectEventCounters(void);

/* Gated 12:00/18:00 event dispatch and one-shot flags 0x3F/0x40. May clear
 * pool time origins and rebuild object events; special scene 1 has its own
 * 18:00 callback path. This is called by the ordinary scene update. */
void Ovl11TriggerTimedObjectEvents(void);

#endif
