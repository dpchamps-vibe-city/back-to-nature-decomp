#ifndef OVL11_PENDING_EVENT_QUEUE_H
#define OVL11_PENDING_EVENT_QUEUE_H

#include "common.h"

/* Two twenty-entry queues use {s16 value, s8 kind, u8 day}; kind -1 is empty.
 * The immediate queue tags entries with the current day. The dated queue
 * stores a countdown byte instead. Enqueue truncates kind/day to bytes;
 * callers must avoid kind -1 and use countdowns 1..127. A full
 * queue drops its oldest entry. This is shared, non-reentrant game state. */

/* Clear only the empty markers and one-shot request flags, not entry payloads
 * or the message-active/latch words. */
void Ovl11ResetEventQueues(void);

/* days==0 uses the immediate queue, even for value zero. Otherwise value zero
 * is ignored. Positive countdowns become immediate entries during service. */
void Ovl11QueueDatedEvent(s16 kind, s16 value, s16 days);

/* Daily service: remove immediate entries tagged with the current day, age
 * dated entries, migrate expired ones, clear the active gate, then evaluate
 * state/date/flag-dependent event requests. Call at the intended day boundary;
 * repeated service calls age countdowns repeatedly. */
void Ovl11ServiceEventQueues(void);

/* Per-frame message progression: active status 2 pops the next entry or
 * completes the special 0x19B latch. Pad bit 0x20 cancels/closes the active
 * window. The special completion updates record 0x103 and a next-day byte. */
void Ovl11UpdateEventQueueGate(void);

/* Idle-only start: returns 1 if a nonempty head was presented (even if the
 * message system has not yet accepted/removed it). An empty queue requests
 * fallback message (2,0x221) and returns 0. Active gate also returns 0. */
s32 Ovl11TryShowQueuedEventMessage(void);

#endif
